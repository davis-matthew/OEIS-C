/*
 * A157774  Numerator of Euler(n, 7/32).
 * https://oeis.org/A157774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 6, i64 n <= 11, i128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A157774: Numerator of Euler(n, 7/32)
   Recurrence:
     a(0) = 1
     a(n) = ( 2*7^n - Σ_{j=0}^{n-1} C(n,j) * 32^{n-j} * a(j) ) / 2   (n ≥ 1)
   The calculation uses 128‑bit intermediates to avoid overflow for 64‑bit
   seq_t builds (terms ≥ n=12 exceed 64 bits anyway).  The cache stores
   all previously computed a(i) together with necessary powers of 7
   and 32.  The function works correctly for any n ≥ 0 and any build
   where seq_t is at least 64 bits. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* external linkage */
seq_t a(idx_t n);

static seq_t *cache = NULL;     /* cached a[i] values */
static size_t cache_len = 0;    /* number of entries filled */

static seq_t *pow2_5 = NULL;    /* 32^k */
static size_t pow2_5_len = 0;

static seq_t *pow7 = NULL;      /* 7^k */
static size_t pow7_len = 0;

/* make sure cache, pow2_5 and pow7 contain entries up to index n */
static void ensure_cache(idx_t n)
{
    /* grow the cache arrays if necessary */
    while (cache_len <= (size_t)n) {
        size_t new_cap = cache_len ? cache_len * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;              /* allocation failure – unlikely */
        cache = tmp;

        /* enlarge pow2_5 */
        if (pow2_5_len <= (size_t)n) {
            size_t new_cap2 = pow2_5_len ? pow2_5_len * 2 : 8;
            seq_t *tmp2 = (seq_t *)realloc(pow2_5, new_cap2 * sizeof(seq_t));
            if (!tmp2) return;
            pow2_5 = tmp2;
            for (size_t i = pow2_5_len; i <= (size_t)n; ++i) {
                if (i == 0) pow2_5[i] = (seq_t)1;
                else pow2_5[i] = pow2_5[i-1] * (seq_t)32;
            }
            pow2_5_len = n + 1;
        }

        /* enlarge pow7 */
        if (pow7_len <= (size_t)n) {
            size_t new_cap3 = pow7_len ? pow7_len * 2 : 8;
            seq_t *tmp3 = (seq_t *)realloc(pow7, new_cap3 * sizeof(seq_t));
            if (!tmp3) return;
            pow7 = tmp3;
            for (size_t i = pow7_len; i <= (size_t)n; ++i) {
                if (i == 0) pow7[i] = (seq_t)1;
                else pow7[i] = pow7[i-1] * (seq_t)7;
            }
            pow7_len = n + 1;
        }

        /* compute a[cache_len] */
        idx_t i = (idx_t)cache_len;
        if (i == 0) {
            cache[cache_len] = (seq_t)1;  /* a(0) = 1 */
            ++cache_len;
            continue;
        }

        __int128 sum128 = 0;
        __int128 binom128 = 1;      /* C(i,0) */
        for (idx_t j = 0; j < i; ++j) {
            __int128 term128 = binom128
                              * (__int128)cache[(size_t)j]
                              * (__int128)pow2_5[(size_t)(i - j)];
            sum128 += term128;

            /* update binomial coefficient for next j */
            if (j + 1 < i) {
                binom128 = binom128 * ( (__int128)(i - j) )
                                / ( (__int128)(j + 1) );
            }
        }

        __int128 numerator128 = (__int128)pow7[(size_t)i] * ( (__int128)2 )
                               - sum128;
        /* division by 2 is exact */
        seq_t ai = (seq_t)(numerator128 / ( (__int128)2 ));

        cache[cache_len] = ai;
        ++cache_len;
    }
}

seq_t a(idx_t n)
{
    /* initialize cache if empty */
    if (cache_len == 0) ensure_cache(0);
    ensure_cache(n);
    return cache[(size_t)n];
}
