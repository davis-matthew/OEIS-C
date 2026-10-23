/*
 * A036058  Summarize digits of preceding number, by decreasing digit value.
 *          Start with a(0) = 0.
 * https://oeis.org/A036058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   digit_counts, digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(0) = 0
 *               a(n+1) = A244112(a(n))   –  summarize digits of a(n) in decreasing
 *               order, writing each count followed by its digit.
 */

#include <stdlib.h>                 /* malloc, realloc */

/* utilities used by the summation */
#include "../utils/digit_counts.h"  /* void digit_counts(seq_t x, unsigned char cnt[10]); */
#include "../utils/digit_len.h"     /* unsigned int digit_len(seq_t x); */
#include "../utils/pow10.h"         /* seq_t pow10(idx_t e); */

/* produce the next term by summarizing the digits of x */
static seq_t summarize(seq_t x)
{
    unsigned char cnt[10] = {0};

    /* count decimal digits of x */
    if (x == (seq_t)0) {
        cnt[0] = 1;                     /* 0 has one zero digit */
    } else {
        digit_counts(x, cnt);
    }

    seq_t res = 0;
    for (int d = 9; d >= 0; --d) {
        if (cnt[d] == 0) continue;

        seq_t c      = (seq_t)cnt[d];               /* count of this digit */
        unsigned int digs = digit_len(c);           /* digits of the count */
        seq_t mult = pow10((idx_t)(digs + 1));      /* 10^(digits+1) */

        /* append c then d:  ... -> ...c d */
        res = res * mult + c * 10 + d;
    }
    return res;
}

/* The sequence function */
seq_t a(idx_t n)
{
    /* simple on‑demand memoization – good for arbitrary call order */
    static seq_t *cache   = NULL;
    static idx_t cache_sz = 0;

    if (cache == NULL) {               /* first call – initialise cache */
        cache     = (seq_t *)malloc(sizeof(seq_t));
        cache_sz  = 1;
        cache[0]  = 0;                  /* a(0) = 0 */
    }

    if (n >= cache_sz) {               /* extend cache up to n */
        idx_t new_sz = n + 1;
        seq_t *tmp   = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (tmp == NULL) {             /* allocation failure: return dummy */
            free(cache);
            cache = NULL;
            cache_sz = 0;
            return (seq_t)0;
        }
        cache = tmp;
        for (idx_t i = cache_sz; i <= n; ++i) {
            cache[i] = summarize(cache[i - 1]);
        }
        cache_sz = new_sz;
    }

    return cache[n];
}
