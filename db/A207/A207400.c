/*
 * A207400  Number of n X 5 0..1 arrays avoiding 0 0 1 and 0 1 1 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A207400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^5 + 9*n^4 + 28*n^3 + 21*n^2 - 29*n + 6)/3
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 86, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Compute the n-th term of OEIS A207400.
 * The formula is a quintic polynomial; it evaluates exactly to an integer.
 * For large n the intermediate n^5 may exceed 64‑bit limits; any build that
 * supports larger seq_t values will handle it correctly.  Small n values
 * (fitting in 64 bits) are also handled safely.
 */
seq_t a(idx_t n)
{
    /* Cast index to the sequence type */
    seq_t x = (seq_t)n;

    /* Compute powers of n */
    seq_t x2 = x * x;          /* n^2 */
    seq_t x3 = x2 * x;         /* n^3 */
    seq_t x4 = x3 * x;         /* n^4 */
    seq_t x5 = x4 * x;         /* n^5 */

    /* Numerator of the fraction */
    seq_t numerator = x5               /* n^5 */
                    + 9 * x4           /* +9*n^4 */
                    + 28 * x3          /* +28*n^3 */
                    + 21 * x2          /* +21*n^2 */
                    - 29 * x           /* -29*n */
                    + 6;               /* +6 */

    /* The result is the integer division by 3 */
    return numerator / 3;
}
