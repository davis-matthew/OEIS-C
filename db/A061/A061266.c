/*
 * A061266  Number of squares between n^3 and (n+1)^3.
 * https://oeis.org/A061266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 if n = 2, otherwise * a(n) = floor(sqrt((n+1)^3 - 1)) -
 *            floor(sqrt(n^3 + 1))
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 63, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt_seq.h"

seq_t a(idx_t n)
{
    /* Handle the special case n = 2 */
    if (n == (idx_t)2)
        return (seq_t)3;

    /* Compute the required squares using 64–bit intermediates to avoid overflow. */
    unsigned long long n_ul  = (unsigned long long)n;
    unsigned long long n_cube  = n_ul * n_ul * n_ul;          /* n^3           */
    unsigned long long n1     = n_ul + 1;
    unsigned long long n1_cube = n1 * n1 * n1;                /* (n+1)^3       */

    /* Apply the integer square root routine on the truncated values. */
    seq_t high = isqrt_seq((seq_t)(n1_cube - 1));   /* floor sqrt((n+1)^3 - 1) */
    seq_t low  = isqrt_seq((seq_t)(n_cube + 1));    /* floor sqrt(n^3 + 1)     */

    return high - low;
}
