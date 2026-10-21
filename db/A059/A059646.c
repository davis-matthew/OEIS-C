/*
 * A059646  Primes p such that x^62 = 2 has no solution mod p.
 * https://oeis.org/A059646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime p for which 2 has no solution to x^62 = 2
 *            (mod p). Equivalently, 2^( (p-1)/gcd(62,p-1) ) ≠ 1 (mod p)
 * Range:     u8 n <= 11, u16 n <= 946, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"
#include "../utils/floor_sqrt.h"

static uint64_t gcd_uint64(uint64_t a, uint64_t b) {
    while (b) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Grow a dynamic array of seq_t elements.  The cast is necessary for C++17. */
static void ensure_cap(seq_t **arr, size_t *cnt, size_t *cap, size_t add) {
    if (*cnt + add > *cap) {
        size_t newcap = (*cap == 0 ? 16 : (*cap << 1));
        while (*cnt + add > newcap)
            newcap <<= 1;
        *arr = (seq_t*)realloc(*arr, newcap * sizeof(seq_t));
        *cap = newcap;
    }
}

/*------------------------------------------------------------------*/
/*   State for incremental generation                              */
/*------------------------------------------------------------------*/
static seq_t *all_primes      = NULL;  /* primes discovered so far          */
static size_t all_cnt         = 0;
static size_t all_cap         = 0;

static seq_t *good_primes     = NULL;  /* primes satisfying the condition  */
static size_t good_cnt        = 0;
static size_t good_cap        = 0;

static seq_t candidate = 3;  /* next odd number to test */

/* Simple primality test using the primes known so far.  The   */
/* sqrt and % operations are available on seq_t.               */
static bool is_prime(seq_t x) {
    if (x < 2)
        return false;
    seq_t sq = floor_sqrt(x);
    for (size_t i = 0; i < all_cnt; ++i) {
        seq_t p = all_primes[i];
        if (p > sq)
            break;
        if (x % p == 0)
            return false;
    }
    return true;
}

/*------------------------------------------------------------------*/
/*   Required OEIS function                                         */
/*------------------------------------------------------------------*/
seq_t a(idx_t n) {
    if (n == 0)
        return 0;  /* undefined for offset zero, but harmless */

    /* Generate primes until we have collected at least n good ones. */
    while (good_cnt < (size_t)n) {

        /* Find the next prime starting from candidate. */
        while (!is_prime(candidate)) {
            candidate += 2;
            if (candidate < 3) /* overflow guard, should not happen */ candidate = 3;
        }

        /* Store the discovered prime. */
        ensure_cap(&all_primes, &all_cnt, &all_cap, 1);
        all_primes[all_cnt++] = candidate;

        /* Test the 62nd power residue condition. */
        seq_t p_minus_1 = candidate - 1;
        uint64_t rem62  = (uint64_t)(p_minus_1 % 62ULL);
        uint64_t g      = gcd_uint64(62ULL, rem62);
        uint64_t exp    = (uint64_t)(p_minus_1 / g);

        seq_t r = mod_pow((seq_t)2, exp, candidate);
        if (r != (seq_t)1) {
            ensure_cap(&good_primes, &good_cnt, &good_cap, 1);
            good_primes[good_cnt++] = candidate;
        }

        /* Advance to the next odd candidate. */
        candidate += 2;
    }

    return good_primes[(size_t)n - 1];
}
