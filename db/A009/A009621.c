/*
 * A009621  Expansion of sinh(x)*cosh(log(1+x)).
 * https://oeis.org/A009621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   Let S(0)=S(1)=0 and, for n≥2, S(n)=n*S(n‑1) if n even, and
 *            S(n)=n*S(n‑1) – n*(n‑1) if n odd. Then a(n)= (n odd?1:0) +
 *            ((-1)^n * S(n))/2 . This yields the coefficients of the series
 *            expansion sinh(x)*cosh(log(1+x))
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *S_cache = NULL;   /* cache of S(n) values   */
static size_t S_size  = 0;      /* allocated length of S_cache */

/* Returns the n-th term of sequence A009621. */
seq_t a(idx_t n)
{
    /* Ensure cache is long enough.  n+1 because we store S[0..n]. */
    if (n >= S_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(S_cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;                /* silently fail if OOM */
        S_cache = tmp;

        /* Initialise new entries in the cache.  */
        for (idx_t i = S_size; i <= n; ++i) {
            if (i <= 1) {
                S_cache[i] = (seq_t)0;
            } else {
                seq_t prev = S_cache[i - 1];
                seq_t prod = (seq_t)i * prev;       /* i*S(i‑1)  */
                if (i & 1ULL) {                    /* i odd  */
                    seq_t sub = (seq_t)i * (i - 1); /* n*(n‑1)   */
                    prod -= sub;                   /* S(n)=n*S(n‑1) – n*(n‑1) */
                }
                S_cache[i] = prod;
            }
        }
        S_size = new_size;
    }

    /* Now compute a(n) from S(n). */
    seq_t delta = ( (n & 1ULL) ? (seq_t)1 : (seq_t)0 );   /* 1 if n odd else 0 */
    seq_t sign  = ( (n & 1ULL) ? (seq_t)-1 : (seq_t)1 );   /* (-1)^n          */

    seq_t term = (sign * S_cache[n]) / (seq_t)2;           /* ((-1)^n * S(n))/2 */
    return delta + term;
}
