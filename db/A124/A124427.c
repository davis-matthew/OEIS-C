/*
 * A124427  Sum of the sizes of the blocks containing the element 1 in all set
 *          partitions of {1,2,...,n}.
 * https://oeis.org/A124427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)*B(n-1) + B(n) where B(m) is the m‑th Bell number.
 *            This function uses a static cache of Bell numbers up to the
 *            largest n encountered. The cache is grown lazily; values are
 *            computed with the recurrence B(0) = 1 B(m) = sum_{k=0}^{m-1}
 *            C(m-1,k) * B(k) . The binomial coefficient is built...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)          /* a(0) = 0 by definition */
        return 0;

    /* Static cache of Bell numbers B[0 .. Bcap-1] */
    static seq_t *B = NULL;
    static idx_t Bcap = 0;

    /* First time: allocate space for B[0] */
    if (B == NULL) {
        B = (seq_t *)malloc(sizeof(seq_t)); /* capacity 1 */
        if (!B) return 0;                   /* allocation failed */
        B[0] = 1;                            /* B(0) = 1 */
        Bcap = 1;
    }

    /* Grow the cache if we need values up to n */
    if (n >= Bcap) {
        seq_t *tmp = (seq_t *)realloc(B, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) return 0;               /* reallocation failed */
        B = tmp;
        /* Compute missing Bell numbers from old capacity up to n */
        idx_t start = Bcap;                /* first missing index */
        Bcap = n + 1;                      /* new capacity */

        for (idx_t m = start; m <= n; ++m) {
            seq_t sum = 0;
            seq_t binom = 1;               /* C(m-1,0) = 1 */

            for (idx_t k = 0; k < m; ++k) {
                sum += binom * B[k];

                if (k < m - 1) {
                    /* binom = binom * (m-1-k) / (k+1) */
                    binom = (binom * (seq_t)(m - 1 - k)) / (seq_t)(k + 1);
                }
            }
            B[m] = sum;
        }
    }

    /* Compute a(n) = (n-1)*B(n-1) + B(n) */
    seq_t prod = ((seq_t)(n - 1)) * B[n - 1];
    return prod + B[n];
}
