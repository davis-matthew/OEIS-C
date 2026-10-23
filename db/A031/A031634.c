/*
 * A031634  Numbers k such that the continued fraction for sqrt(k) has odd
 *          period and if the last term of the periodic part is deleted the
 *          two central terms are both 46.
 * https://oeis.org/A031634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A031634
   Numbers k such that the continued fraction for √k has an odd period
   and after deleting the last term of the periodic part the two
   central terms are both 46.
   The function `a` computes the nth term by brute‑force searching
   increasing k and memoising found values.  */
/* This implementation is valid in both C11 and C++17. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>          /* for sqrtl */

static seq_t *cache = NULL;    /* memoised sequence values */
static size_t cache_cap = 0, cache_len = 0;
static uint64_t last_k = 1;    /* next candidate k to test */

/* Ensure room for one more element in cache. */
static void grow_cache(void)
{
    if (cache_cap == 0) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_len == cache_cap) {
        cache_cap <<= 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
}

/* Integer square root of a 64‑bit value.  */
static inline uint64_t uint64_sqrt(uint64_t x)
{
    /* Use long double sqrt for initial guess, then correct. */
    uint64_t r = (uint64_t)sqrtl((long double)x);
    while ((unsigned __int128)(r + 1ULL) * (r + 1ULL) <= x) ++r;
    while ((unsigned __int128)r * r > x) --r;
    return r;
}

/* Test whether an integer is a perfect square. */
static inline bool is_square(uint64_t x)
{
    uint64_t r = uint64_sqrt(x);
    return r * r == x;
}

/* Compute the nth term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but harmless */

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        for (uint64_t k = last_k; ; ++k) {
            if (is_square(k)) continue;       /* skip perfect squares */

            uint64_t a0 = uint64_sqrt(k);

            /* Generate the periodic part of the continued fraction. */
            /* Period length will be at most 200000 for our search range. */
            static uint64_t period[200000];
            size_t L = 0;

            uint64_t m = 0, d = 1, a_term = a0;
            do {
                m = d * a_term - m;
                d = (unsigned __int128)(k - (unsigned __int128)m * m) / d;
                a_term = (a0 + m) / d;
                period[L++] = a_term;
            } while (a_term != 2ULL * a0);

            /* Check conditions for the sequence. */
            if ((L & 1) == 0) continue;     /* period must be odd */

            size_t sublen = L - 1;          /* length after deleting last term */
            if (sublen < 2) continue;       /* need at least two central terms */

            size_t mid1 = sublen / 2 - 1;
            size_t mid2 = sublen / 2;

            if (period[mid1] == 46 && period[mid2] == 46) {
                /* Found a valid term. */
                grow_cache();
                cache[cache_len++] = (seq_t)k;

                if (cache_len >= (size_t)n) {
                    last_k = k + 1;
                    return cache[n - 1];
                }
                last_k = k + 1;
                break;                     /* next k in outer search */
            }
            /* Otherwise continue searching for next k. */
        }
    }
    return cache[n - 1];
}
