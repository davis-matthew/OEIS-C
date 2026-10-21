/*
 * A213779  Principal diagonal of the convolution array A213778.
 * https://oeis.org/A213779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n^3 + 15*n^2 + 10*n)/24 if n even a(n) = (8*n^3 +
 *            15*n^2 + 4*n - 3)/24 if n odd (OEIS A213779)
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 910, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* offset starts at 1 */

    seq_t tn  = (seq_t)n;            /* cast once to seq_t */
    seq_t n2  = tn * tn;             /* n^2 */
    seq_t n3  = n2 * tn;             /* n^3 */

    if (n & 1) /* odd */
    {
        seq_t num = (8 * n3) + (15 * n2) + (4 * tn) - 3;
        return num / 24;
    }
    else /* even */
    {
        seq_t num = (8 * n3) + (15 * n2) + (10 * tn);
        return num / 24;
    }
}
