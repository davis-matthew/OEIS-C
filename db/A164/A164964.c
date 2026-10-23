/*
 * A164964  Number of reduced words of length n in Coxeter group on 26
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^8 = I.
 * https://oeis.org/A164964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (derived from the rational G.f.): 
   Let c[0..8] = {1,2,2,2,2,2,2,2,1}  (the numerator coefficients).  
   For n ≥ 0:

   a(n) = 24*(a(n-1)+a(n-2)+…+a(n-7)) – 300*a(n-8) + c[n]
   (terms a(k)=0 for k<0; for n>8, c[n]=0).

   This generates the OEIS sequence A164964. */
#include <stdlib.h>

static seq_t *cache = NULL;     /* memoisation array */
static idx_t   cap   = 0;        /* allocated size */
static idx_t   high  = 0;        /* highest index computed (inclusive) */

static const unsigned int c[9] = {
    1, /* n = 0 */
    2, /* n = 1 */
    2, /* n = 2 */
    2, /* n = 3 */
    2, /* n = 4 */
    2, /* n = 5 */
    2, /* n = 6 */
    2, /* n = 7 */
    1  /* n = 8 */
};

seq_t a(idx_t n)
{
    /* initialise cache on first use */
    if (!cache) {
        /* initial capacity at least 9 (indices 0..8) */
        size_t init_cap = 16;
        cache = (seq_t *)realloc(NULL, init_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;
        cap = (idx_t)init_cap;
        high = (idx_t)-1;  /* no term computed yet */
    }

    /* enlarge if necessary */
    if (n + 1 > cap) {
        size_t new_cap = (size_t)(n + 1);
        if (new_cap < 16) new_cap = 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cap = (idx_t)new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = high + 1; i <= n; ++i) {
        /* sum of last up to 7 terms */
        seq_t sum = 0;
        for (idx_t k = 1; k <= 7; ++k) {
            if (i >= k) sum += cache[i - k];
        }

        /* recurrence part */
        seq_t term = 24 * sum;
        if (i >= 8) {
            term -= 300 * cache[i - 8];
        }

        /* add numerator coefficient if within its range */
        if (i <= 8) {
            term += (seq_t)c[i];
        }

        cache[i] = term;
    }

    high = n;
    return cache[n];
}
