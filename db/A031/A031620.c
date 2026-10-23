/*
 * A031620  Numbers k such that the continued fraction for sqrt(k) has odd
 *          period and if the last term of the periodic part is deleted the
 *          two central terms are both 32.
 * https://oeis.org/A031620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   iexact_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A031620: numbers k such that
 * the continued fraction of √k has an odd period; if the last
 * term of the periodic part is deleted, the two central terms of
 * the remaining even-length period are both 32.
 *
 * a(n) returns the n‑th such integer k (1‑based indexing).
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/iexact_isqrt_u64.h"

/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*  Cache for discovered terms.  Cache grows geometrically.         */
static seq_t *cache = NULL;          /* pointer to cache array          */
static size_t cache_len = 0;         /* number of terms stored          */
static size_t cache_cap = 0;         /* allocated capacity              */
static idx_t processed_up_to = 1;    /* largest k already examined      */

/*------------------------------------------------------------------*/
static void cache_append(seq_t val)
{
    if (cache_len >= cache_cap) {
        cache_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_len++] = val;
}

/*------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Extend cache until it contains at least n terms. */
    while (cache_len < n) {
        /* Scan successive k values that have not yet been examined. */
        for (idx_t k = processed_up_to + 1; ; ++k) {
            processed_up_to = k;

            /* Skip perfect squares. */
            idx_t a0 = iexact_isqrt_u64(k);
            if (a0 * a0 == k) continue;

            /* Compute continued fraction of √k. */
            idx_t m = 0;
            idx_t d = 1;
            idx_t a  = a0;

            /* Store the periodic partial quotients (including the final 2*a0). */
            /* Period length will be the number of values stored. */
            unsigned int cf_terms[4096];   /* 4K terms are more than enough for 64‑bit k. */
            unsigned int len = 0;

            while (1) {
                /* Use 128‑bit temporaries to avoid overflow. */
                __uint128_t dm = (__uint128_t)d * (__uint128_t)a;
                m = (idx_t)(dm - m);      /* m = d*a - m */

                /* m*m fits in 128 bits. */
                __uint128_t mm = (__uint128_t)m * (__uint128_t)m;
                __uint128_t diff = (__uint128_t)k - mm;
                d = (idx_t)(diff / d);    /* d = (k - m*m) / d */

                a = (idx_t)( (a0 + m) / d );   /* a = (a0 + m) / d */

                cf_terms[len++] = (unsigned int)a;

                if (a == 2 * a0)          /* end of period detected */
                    break;
                /* Period length for 64‑bit k never exceeds a few thousand,
                   so a 4K buffer is sufficient. */
            }

            /* Property: odd period and two central terms are 32 after deleting
               the final term.  len is odd (period length), so len-1 is even. */
            if ((len & 1) && len >= 3) {
                unsigned int pos = (len - 1) >> 1;     /* zero‑based index of second middle after deletion */
                if (cf_terms[pos - 1] == 32 && cf_terms[pos] == 32) {
                    cache_append((seq_t)k);
                    break;               /* found next term; break the outer scan loop */
                }
            }
        }
    }

    return cache[n - 1];
}
