/*
 * A129901  For two consecutive primes p and q the difference 10*q - p is
 *          prime. The first of the pair of primes is listed.
 * https://oeis.org/A129901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 656, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9828 (2026-11-03)
 * Helpers:   is_prime_u64, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iterate over consecutive primes p and q;
   if 10*q - p is prime, record p.  Return the n‑th recorded prime. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Utility headers – they provide the headers for is_prime_big
   and next_prime used in this implementation. */
#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Returns the n‑th term of OEIS A129901 (1‑based). */
seq_t a(idx_t n)
{
    static seq_t *primes = NULL;     /* cached qualifying primes p     */
    static size_t capacity = 0;      /* allocated capacity of primes   */
    static size_t count   = 0;       /* number of primes stored        */
    static uint64_t last_prime = 0;  /* last prime examined (0 → start) */

    /* Grow the cache until it contains at least n elements. */
    while (count < (size_t)n) {
        /* Find the next two consecutive primes. */
        uint64_t p = next_prime(last_prime);   /* current prime p */
        uint64_t q = next_prime(p);            /* next prime q   */

        /* t = 10 * q - p  (all arithmetic done in seq_t). */
        seq_t t = (seq_t)10 * (seq_t)q - (seq_t)p;

        /* If t is prime, store p in the cache. */
        if (is_prime_big(t)) {
            if (count == capacity) {
                size_t newcap = capacity ? capacity * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, primes, newcap);
                if (!tmp) {
                    /* Allocation failure – return 0 (undefined in OEIS). */
                    return 0;
                }
                primes = tmp;
                capacity = newcap;
            }
            primes[count++] = (seq_t)p;
        }

        /* Prepare for the next iteration. */
        last_prime = p;
    }

    /* Return the requested term (a(1) is at index 0). */
    return primes[(size_t)n - 1];
}
