/*
 * A053223  Second differences of sigma(n).
 * https://oeis.org/A053223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sigma(n+2) - 2*sigma(n+1) + sigma(n) (offset 1)
 * Range:     i8 n <= 46, i16 n <= 6718, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Compute divisor sums sigma(i) for i = 0 .. n+2 using a sieve. */
    idx_t M = n + 2;                     /* maximum index needed    */
    size_t len = (size_t)(M + 1);        /* array length (0..M)     */
    seq_t *sigma = (seq_t*)calloc(len, sizeof(seq_t));
    if (!sigma) return (seq_t)0;         /* allocation error fallback */

    for (idx_t i = 1; i <= M; ++i) {
        seq_t val = (seq_t)i;
        for (idx_t j = i; j <= M; j += i)
            sigma[j] += val;
    }

    seq_t result = sigma[n + 2]
                 - ( (seq_t)2 * sigma[n + 1] )
                 + sigma[n];

    free(sigma);
    return result;
}
