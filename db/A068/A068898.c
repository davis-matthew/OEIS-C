/*
 * A068898  Triangular numbers containing 2k digits in which the sum of the
 *          first k digits = that of the rest.
 * https://oeis.org/A068898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sum_digits_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A068898:
 *  Triangular numbers T_n = n(n+1)/2 that contain an even number of decimal
 *  digits and for which the sum of the first half of the digits equals the
 *  sum of the second half. This function computes the nth such triangular
 *  number (offset 1).  It performs a linear scan of triangular numbers
 *  and stores all found terms in a cache so that repeated, out‑of‑order
 *  calls are efficient.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/sum_digits_seq_t.h"

/* --------------------------------------------------------------------- */
/* Helper: number of decimal digits of a seq_t value                   */
static int digit_len_seq_t(seq_t v)
{
    /* For v > 0; v == 0 would have 1 digit (not reached here).          */
    int len = 0;
    while (v != 0) {
        ++len;
        v /= (seq_t)10;
    }
    return len;
}

/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* Cache of found terms ------------------------------------------------ */
static seq_t *term_cache = NULL;   /* array of sequence values          */
static size_t cache_len  = 0;      /* number of terms stored            */
static size_t cache_cap  = 0;      /* allocated capacity                */

/* Append a value to the cache, expanding if necessary */
static void cache_append(seq_t val)
{
    if (cache_len + 1 > cache_cap) {
        size_t new_cap = cache_cap == 0 ? 4 : cache_cap * 2;
        seq_t *new_buf = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
        if (!new_buf) return;        /* allocation failure – give up silently */
        term_cache = new_buf;
        cache_cap  = new_cap;
    }
    term_cache[cache_len++] = val;
}

/* --------------------------------------------------------------------- */
/* Cache of powers of ten ------------------------------------------------*/
#define MAX_POW10 512
static seq_t pow10[MAX_POW10];
static size_t pow10_cap = 0;        /* number of powers precomputed       */

/* Ensure that 10^k is available in pow10[k] (k < MAX_POW10).          */
static void ensure_pow10(size_t k)
{
    if (k >= MAX_POW10) return;      /* should not happen for requested n */
    if (pow10_cap == 0) {
        pow10[0] = (seq_t)1;
        pow10_cap = 1;
    }
    while (pow10_cap <= k) {
        pow10[pow10_cap] = pow10[pow10_cap - 1] * (seq_t)10;
        ++pow10_cap;
    }
}

/* --------------------------------------------------------------------- */
/* Progress across triangular numbers ------------------------------------*/
static idx_t last_tri_idx = 0;      /* last triangular index processed   */

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, but guard against 0 */

    /* Extend the cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        ++last_tri_idx;

        /* T = last_tri_idx*(last_tri_idx+1)/2 */
        seq_t tri = (seq_t)last_tri_idx * ((seq_t)last_tri_idx + (seq_t)1) / (seq_t)2;

        int len = digit_len_seq_t(tri);
        if ((len & 1) != 0) continue;          /* odd number of digits -> skip */

        size_t k = len / 2;
        ensure_pow10(k);

        seq_t first  = tri / pow10[k];
        seq_t second = tri % pow10[k];

        if (sum_digits_seq_t(first) == sum_digits_seq_t(second)) {
            cache_append(tri);                 /* store the term */
        }
    }

    return term_cache[n - 1];
}
