/*
 * A152678  Even members of A000203.
 * https://oeis.org/A152678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sigma(A028983(n)) = A000203(A028983(n)) Where A028983(n)
 *            is the n-th integer m for which sigma(m) is even. Thus a(n) is
 *            the sigma(m) values for all m whose divisor sum is even, listed
 *            in ascending order of m
 * Range:     u8 n <= 90, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A152678. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1 */

    /* estimate a limit large enough to contain the first n even-sigma numbers */
    idx_t limit = (n <= 1) ? 3 : n * 2 + 10;   /* start with a rough bound */

    while (1) {
        /* allocate sigma array of size limit+1, indexed from 0..limit */
        seq_t *sigma = (seq_t *)malloc((size_t)(limit + 1) * sizeof(seq_t));
        if (!sigma) return (seq_t)0; /* allocation failure, return 0 */

        /* initialise to zero */
        for (idx_t i = 0; i <= limit; ++i) sigma[i] = 0;

        /* compute sigma values via a simple divisor sieve */
        for (idx_t d = 1; d <= limit; ++d) {
            for (idx_t m = d; m <= limit; m += d) {
                sigma[m] += (seq_t)d;
            }
        }

        /* count even-sigma numbers and find the n-th one */
        idx_t count = 0;
        seq_t result = 0;
        for (idx_t i = 1; i <= limit; ++i) {
            if ((sigma[i] & 1) == 0) {  /* sigma[i] is even */
                count++;
                if (count == n) {
                    result = sigma[i];
                    break;
                }
            }
        }

        free(sigma);

        if (count >= n) return result;           /* success */
        /* not enough terms yet: double the limit and try again */
        limit <<= 1;
    }
}
