/*
 * A166566  Primes p such that product of digits - 1 is prime.
 * https://oeis.org/A166566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th prime p such that digit_product(p)−1 is prime
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 965 (2026-11-03)
 * Helpers:   digit_product, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_product.h"
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

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    static seq_t *seq   = NULL;   /* cache of found terms             */
    static idx_t len    = 0;      /* number of terms stored           */
    static size_t cap   = 0;      /* allocated capacity of seq        */
    static idx_t next_candidate = 3; /* candidate primes to test (start at 3) */

    if (n == 0) return 0;          /* offset is 1; 0 is a harmless value */

    /* Return cached value if already computed. */
    if (n <= len) return seq[n - 1];

    /* Generate terms until we have at least n of them. */
    while (len < n) {
        /* Search for the next prime with the required property. */
        while (true) {
            idx_t cand = next_candidate++;          /* candidate integer */

            if (!is_prime_big((seq_t)cand)) continue;   /* must be prime */

            seq_t prod = digit_product((seq_t)cand);    /* product of digits */
            if (prod == (seq_t)0 || prod == (seq_t)1) continue;

            seq_t val = prod - (seq_t)1;                /* prod-1           */
            if (!is_prime_big(val)) continue;           /* val must be prime */

            /* Grow cache if necessary. */
            if (len + 1 > cap) {
                size_t newcap = cap ? cap * 2 : 32;
                seq_t *tmp = seq_realloc(seq, cap, newcap);
                if (!tmp) abort();   /* out of memory */
                seq = tmp;
                cap = newcap;
            }

            seq[len++] = (seq_t)cand;          /* store the found prime */
            break;                             /* move on to next term   */
        }
    }

    return seq[n - 1];
}
