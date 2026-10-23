/*
 * A159571  Fifth diagonal in Table A159572; previous diagonals are A000012,
 *          A000217, A000330 and A086602.
 * https://oeis.org/A159571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (13*n^4 - 8*n^3 + 119*n^2 - 100*n + 48) / 12
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = sum of the four sequences term by term:
            1 (A000012) + n(n+1)/2 (A000217) + n(n+1)(2n+1)/6 (A000330)
            + (13 n^4 - 12 n^3 + 107 n^2 -108 n +36)/12 (A086602)   */

extern seq_t a(idx_t n)
{
    /* Cast n to seq_t first to avoid possible overflow in intermediate multiplications */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;           /* n^2 */
    seq_t n3 = n2 * nn;           /* n^3 */
    seq_t n4 = n3 * nn;           /* n^4 */

    seq_t num = 13 * n4            /* 13*n^4 */
            - 8 * n3              /* -8*n^3 */
            + 119 * n2            /* +119*n^2 */
            - 100 * nn            /* -100*n  */
            + 48;                 /* +48    */

    return num / 12;
}
