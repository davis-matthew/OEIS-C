/*
 * A058562  Another 3-way generalization of series-parallel networks with n
 *          labeled edges.
 * https://oeis.org/A058562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1, a(n)=a(n-1)+Σ_{j=1}^{n-1}
 *            binomial(n,j)*a(j)*a(n-j) (n>1)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* memoised array of previously computed values */
    static seq_t *arr = NULL;   /* array of a(i)  */
    static size_t cap = 0;      /* current capacity   */
    static idx_t sz  = 0;       /* number of entries stored */

    /* trivial cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* initialise the array on the first call */
    if (!arr) {
        cap = 4;                       /* small start, will grow automatically */
        arr = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!arr) return (seq_t)0;     /* allocation failure – conservative */
        arr[0] = (seq_t)0;
        arr[1] = (seq_t)1;
        sz = 2;                       /* we have stored indices 0 and 1 */
    }

    /* grow the array if necessary */
    if ((size_t)n >= cap) {
        while (cap <= (size_t)n) cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(arr, cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* allocation failure */
        arr = tmp;
    }

    /* compute any missing values up to n */
    for (idx_t k = sz; k <= n; ++k) {
        if (k <= 1) { arr[k] = (k==0?0:1); continue; }

        seq_t sum = (seq_t)0;
        idx_t half = k / 2;           /* k/2 rounded down          */

        for (idx_t j = 1; j <= half; ++j) {
            idx_t l = k - j;          /* complementary index      */
            seq_t prod = arr[j];
            prod *= arr[l];
            prod *= binom(k, j);      /* binomial coefficient      */

            sum += prod;              /* term for j                 */
            if (j != l) sum += prod;  /* symmetric partner – double */
        }

        arr[k] = arr[k-1] + sum;
    }

    sz = n + 1;                       /* all indices 0..n are now stored */
    return arr[n];
}
