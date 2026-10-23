/*
 * A173687  Numbers m such that the sum of factorial of the decimal digits of
 *          m is square.
 * https://oeis.org/A173687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 43, u32 n <= 9700, u64 n <= 9700,
 *            u128 n <= 9700, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A173687: numbers m such that the sum of factorials of
 * decimal digits of m is a perfect square. The sequence is generated
 * by brute‑force search with caching, suitable for 64‑bit or larger
 * seq_t types and arbitrary‑precision builds. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Precomputed factorials for decimal digits 0–9, cast to seq_t. */
static const seq_t _fact[10] = {
    (seq_t)1ULL,          /* 0! */
    (seq_t)1ULL,          /* 1! */
    (seq_t)2ULL,          /* 2! */
    (seq_t)6ULL,          /* 3! */
    (seq_t)24ULL,         /* 4! */
    (seq_t)120ULL,        /* 5! */
    (seq_t)720ULL,        /* 6! */
    (seq_t)5040ULL,       /* 7! */
    (seq_t)40320ULL,      /* 8! */
    (seq_t)362880ULL      /* 9! */
};

/* Integer square root for 64‑bit unsigned integers.  The loop uses
 * division to avoid overflow when checking mid*mid <= v. */
static inline unsigned long long isqrt_ull(unsigned long long v)
{
    unsigned long long lo = 0, hi = v, ans = 0;
    while (lo <= hi) {
        unsigned long long mid = (lo + hi) >> 1;
        if (mid == 0 || mid <= v / mid) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    /* Cache of the sequence terms found so far. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Next integer to test; start with 0 so a(1) = 0. */
    static idx_t next_to_test = 0;

    /* Generate terms until we have at least n values. */
    while (cache_len < n) {
        /* Compute sum of factorials of decimal digits of m. */
        idx_t m = next_to_test;
        unsigned long long sum = 0;
        if (m == 0) {
            sum = 1ULL;                /* 0! = 1 */
        } else {
            idx_t x = m;
            while (x > 0) {
                int d = x % 10;
                sum += (unsigned long long)_fact[d];
                x /= 10;
            }
        }

        /* Check whether sum is a perfect square. */
        unsigned long long r = isqrt_ull(sum);
        if (r * r == sum) {
            /* Ensure capacity before adding. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                cache = tmp;
                cache_cap = new_cap;
            }
            /* Store the term; cast assured to be exact for seq_t. */
            cache[cache_len++] = (seq_t)m;
        }

        ++next_to_test;
    }

    return cache[n - 1];
}
