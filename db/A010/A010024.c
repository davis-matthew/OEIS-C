/*
 * A010024  Coordination sequence for squashed {D_5}* lattice, perhaps the
 *          smallest example of a "non-superficial" lattice.
 * https://oeis.org/A010024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10/3*n^4-5/3*n^3+20/3*n^2+5/3*n+2 for n>0; a(0)=1
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 188, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t n4 = n3 * nn;          /* n^4 */

    seq_t num = 10 * n4;         /* 10*n^4 */
    num = num - 5 * n3;          /* -5*n^3 */
    num = num + 20 * n2;         /* +20*n^2 */
    num = num + 5 * nn;          /* +5*n */
    num = num + 6;               /* +6   */

    return num / 3;
}
