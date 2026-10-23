/*
 * A013069  Expansion of e.g.f.:
 *          exp(arcsinh(x)+log(x+1))=1+2*x+3/2!*x^2+3/3!*x^3-3/4!*x^4-15/5!*x^5...
 * https://oeis.org/A013069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 14, i64 n <= 22, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A013069: Expansion of exp(arcsinh(x)+log(1+x))
   Recurrence (derived from the D‑finite relation):
      n*(n-2)*(n+2)*(n+1)*a(n) +
      (n+2)*(2*n^2+2*n-3)*a(n+1) +
      (2*n+3)*(n+1)*a(n+2) +
      2*(n+1)*a(n+3) + a(n+4) = 0
   Initial values: a(0)=1, a(1)=2, a(2)=3, a(3)=3
   (This recurrence holds for all n≥0.)
*/

#include <stdlib.h>
#include <stdint.h>

#ifndef __SIZEOF_INT128__
#  define USE_NATIVE_128 0
#else
#  define USE_NATIVE_128 1
#endif

/* Helper to multiply two seq_t values safely.
   For native integral seq_t types it uses a 128‑bit intermediate
   when available.  For arbitrary‑precision types it falls back
   to the native multiply operator. */
static inline seq_t safe_mul(seq_t a, seq_t b)
{
#if USE_NATIVE_128
    if (sizeof(seq_t) <= sizeof(long long))
        return (seq_t)(((__int128)a) * (__int128)b);
#endif
    return a * b;
}

seq_t a(idx_t n)
{
    /* static memoization array */
    static seq_t *memo = NULL;
    static idx_t memo_len = 0;   /* number of stored entries (indices 0..len-1) */
    static idx_t memo_cap = 0;   /* allocated capacity */

    /* Ensure capacity for indices up to n */
    if (n + 1 > memo_cap) {
        idx_t new_cap = (n + 1) * 2;
        if (new_cap < 4) new_cap = 4;   /* must accommodate the first 4 terms */
        seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        memo = tmp;
        memo_cap = new_cap;
    }

    /* initialise first terms if table is empty */
    if (memo_len == 0) {
        memo[0] = (seq_t)1;
        memo[1] = (seq_t)2;
        memo[2] = (seq_t)3;
        memo[3] = (seq_t)3;
        memo_len = 4;
    }

    /* if already computed, return cached value */
    if (n < memo_len) return memo[n];

    /* compute missing values up to n */
    for (idx_t i = memo_len; i <= n; ++i) {
        if (i < 4) {      /* safety: though i starts at >=4 */
            continue;
        }

        idx_t idx = i - 4;         /* use n = idx in recurrence */
        seq_t nseq = (seq_t)idx;

        /* term0 = n*(n-2)*(n+2)*(n+1)*a(n) */
        seq_t term0 = safe_mul(nseq, nseq - (seq_t)2);
        term0 = safe_mul(term0, nseq + (seq_t)2);
        term0 = safe_mul(term0, nseq + (seq_t)1);
        term0 = safe_mul(term0, memo[idx]);

        /* term1 = (n+2)*(2*n^2+2*n-3)*a(n+1) */
        seq_t n2   = safe_mul(nseq, nseq);                     /* n^2 */
        seq_t t1_1 = safe_mul((seq_t)2, n2);                   /* 2*n^2 */
        seq_t t1_2 = safe_mul((seq_t)2, nseq);                 /* 2*n  */
        seq_t coeff1 = t1_1 + t1_2 - (seq_t)3;                  /* 2*n^2+2*n-3 */
        seq_t term1 = safe_mul(nseq + (seq_t)2,
                              safe_mul(coeff1, memo[idx + 1]));

        /* term2 = (2*n+3)*(n+1)*a(n+2) */
        seq_t two_n    = safe_mul((seq_t)2, nseq);
        seq_t term2_1  = two_n + (seq_t)3;                    /* 2*n+3 */
        seq_t term2_2  = nseq + (seq_t)1;                    /* n+1   */
        seq_t term2    = safe_mul(term2_1,
                                  safe_mul(term2_2, memo[idx + 2]));

        /* term3 = 2*(n+1)*a(n+3) */
        seq_t term3    = safe_mul((seq_t)2,
                                  safe_mul(nseq + (seq_t)1, memo[idx + 3]));

        /* a(n+4) = -(term0 + term1 + term2 + term3) */
        seq_t sum = term0 + term1 + term2 + term3;
        memo[i] = -(sum);
    }

    memo_len = n + 1;
    return memo[n];
}
