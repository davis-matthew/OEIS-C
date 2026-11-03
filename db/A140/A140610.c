/*
 * A140610  Running prime totals of prime factors (without multiplicity) of
 *          consecutive composite N.
 * https://oeis.org/A140610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the n‑th running sum of distinct prime factors of
 *            consecutive composite integers that is itself prime
 * Range:     u8 n <= 9, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-03)
 * Helpers:   gcd_u64, is_prime, is_prime_u64, mem, mulmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "../utils/gcd_u64.h"
#include "../utils/is_prime.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mulmod.h"
#include "../utils/mem.h"

/* Pollard–Rho: find a non‑trivial divisor of n (n composite) */
static uint64_t pollard(uint64_t n) {
    if (n % 2 == 0) return 2;
    uint64_t c = 1;
    while (1) {
        uint64_t x = 2, y = 2, d = 1;
        while (d == 1) {
            x = (mulmod(x, x, n) + c) % n;
            y = (mulmod(y, y, n) + c) % n;
            y = (mulmod(y, y, n) + c) % n;
            uint64_t diff = x > y ? x - y : y - x;
            d = gcd_u64(diff, n);
        }
        if (d != n) return d;
        ++c;  /* try another constant */
    }
}

/* recursive smallest prime factor */
static uint64_t smallest_prime_factor(uint64_t n) {
    if (n % 2 == 0) return 2;
    if (is_prime_u64(n)) return n;
    uint64_t d = pollard(n);
    uint64_t s1 = smallest_prime_factor(d);
    uint64_t s2 = smallest_prime_factor(n / d);
    return (s1 < s2) ? s1 : s2;
}

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

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)0;          /* undefined offset */

    /* static cache for computed terms */
    static seq_t *terms   = NULL;
    static size_t terms_cap = 0;
    static size_t terms_len = 0;
    /* running state */
    static idx_t  next_comp   = 4;   /* first composite */
    static seq_t running_sum  = 0;   /* running sum of distinct prime factors */

    /* allocate terms array on first call */
    if (!terms) {
        terms_cap = 128;
        terms = mem_new(seq_t, terms_cap);
        if (!terms) return (seq_t)0;   /* allocation failed */
    }

    /* generate terms until we have at least n of them */
    while (terms_len < (size_t)n) {
        /* skip over primes */
        while (is_prime(next_comp))
            ++next_comp;

        /* sum distinct prime factors of next_comp */
        uint64_t x = (uint64_t)next_comp;
        seq_t sum_dist = 0;
        while (x > 1) {
            uint64_t p = smallest_prime_factor(x);
            sum_dist += (seq_t)p;
            while (x % p == 0)
                x /= p;
        }

        /* update the running sum */
        running_sum += sum_dist;

        /* record a term if the running sum itself is prime */
        if (is_prime_big(running_sum)) {
            if (terms_len == terms_cap) {
                terms_cap <<= 1;
                seq_t *tmp = mem_resize(seq_t, terms, terms_cap);
                if (!tmp) return (seq_t)0;  /* realloc failed */
                terms = tmp;
            }
            terms[terms_len++] = running_sum;
        }

        /* advance to the next integer */
        ++next_comp;
    }

    return terms[n - 1];
}
