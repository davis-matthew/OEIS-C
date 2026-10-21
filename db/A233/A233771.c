/*
 * A233771  Duplicate of A210249.
 * https://oeis.org/A233771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = A000041(n) – A025065(n) (non‑palindromic partitions of n)
 * Range:     i8 n <= 14, i16 n <= 39, i32 n <= 47, i64 n <= 47, i128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *p_cache = NULL;      /* partition numbers p(k)      */
static seq_t *pref_cache = NULL;   /* prefix sums of p(k)          */
static idx_t  max_n   = 0;          /* highest n that has been computed */

/* Extend the partition and prefix arrays up to n, if necessary */
static void ensure_arrays(idx_t n)
{
    if (n <= max_n) return;

    /* Reallocate arrays to hold 0..n */
    seq_t *new_p   = (seq_t *)realloc(p_cache,  (n + 1) * sizeof(seq_t));
    seq_t *new_pref= (seq_t *)realloc(pref_cache,(n + 1) * sizeof(seq_t));
    if (!new_p || !new_pref) abort();          /* allocation failed */

    p_cache   = new_p;
    pref_cache= new_pref;

    if (max_n == 0) {                         /* initialize p[0] and pref[0] */
        p_cache[0]   = (seq_t)1;
        pref_cache[0]= (seq_t)1;
    }

    /* Compute partition numbers p(m) for m = max_n+1 .. n */
    for (idx_t m = max_n + 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; ; ++k) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > m) break;
            if (k & 1)  sum += p_cache[m - g1];
            else        sum -= p_cache[m - g1];

            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 > m) continue;
            if (k & 1)  sum += p_cache[m - g2];
            else        sum -= p_cache[m - g2];
        }
        p_cache[m] = sum;
    }

    /* Compute prefix sums pref(m) = p[0] + ... + p[m] */
    for (idx_t m = max_n + 1; m <= n; ++m) {
        pref_cache[m] = pref_cache[m - 1] + p_cache[m];
    }

    max_n = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;         /* the offset is 1 */
    ensure_arrays(n);

    seq_t val = p_cache[n];              /* A000041(n) */

    if ((n & 1) == 0) {                  /* subtract p(n/2) if n even */
        val -= p_cache[n / 2];
    }

    idx_t k = (n - 1) >> 1;              /* k = floor((n-1)/2) */
    val -= pref_cache[k];                /* subtract sum_{j=0}^{k} p(j) */

    return val;                          /* a(n) = A000041(n) – A025065(n) */
}
