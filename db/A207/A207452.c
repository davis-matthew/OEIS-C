/*
 * A207452  Number of n X 7 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A207452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 168*n^4 - 84*n^3 - 84*n^2 + 42*n (valid for n >= 1,
 *            offset 1) This implementation uses the closed‑form polynomial,
 *            which is O(1)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 71, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) /* offset is 1, but we return 0 for safety */
        return (seq_t)0;

    /* Cast to seq_t to perform calculations in the target integer type */
    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;          /* n^2 */
    seq_t N3 = N2 * N;         /* n^3 */
    seq_t N4 = N2 * N2;        /* n^4 */

    /* Compute each term of the polynomial */
    seq_t t1 = (seq_t)168 * N4;
    seq_t t2 = (seq_t)84  * N3;
    seq_t t3 = (seq_t)84  * N2;
    seq_t t4 = (seq_t)42  * N;

    /* Combine terms: all intermediate subtractions stay non‑negative,
       so unsigned arithmetic is safe. */
    return t1 - t2 - t3 + t4;
}
