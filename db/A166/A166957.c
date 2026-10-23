/*
 * A166957  a(n) = 841*n^3 + 261*n^2 + 28*n + 1.
 * https://oeis.org/A166957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 172, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 841*n^3 + 261*n^2 + 28*n + 1
 */
seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;          /* convert index to the sequence type */
    seq_t sn2 = sn * sn;          /* n^2 */
    seq_t sn3 = sn2 * sn;         /* n^3 */

    seq_t result = (seq_t)841 * sn3;   /* 841*n^3 */
    result += (seq_t)261 * sn2;       /* +261*n^2 */
    result += (seq_t)28  * sn;        /* +28*n */
    result += (seq_t)1;               /* +1 */

    return result;
}
