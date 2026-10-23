/*
 * A126276  Moment of inertia of all magic cubes of order n.
 * https://oeis.org/A126276
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^3)*(n^3 + 1)*(n^2 - 1)/12
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 21, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* For offsets below 2 the sequence is undefined; return 0 */
    if (n < 2) return (seq_t)0;

    /* Cast n to seq_t to avoid integer promotion issues */
    seq_t N = (seq_t)n;
    seq_t N2 = N * N;
    seq_t N3 = N2 * N;

    /* Three factors of the numerator */
    seq_t A = N3;           /* n^3 */
    seq_t B = N3 + (seq_t)1;/* n^3 + 1 */
    seq_t C = N2 - (seq_t)1;/* n^2 - 1 */

    /* Divide one factor by 4 (avoid overflow) */
    if (A % (seq_t)4 == (seq_t)0) {
        A /= (seq_t)4;
    } else if (B % (seq_t)4 == (seq_t)0) {
        B /= (seq_t)4;
    } else {
        C /= (seq_t)4;
    }

    /* Divide a (remaining) factor by 3 */
    if (A % (seq_t)3 == (seq_t)0) {
        A /= (seq_t)3;
    } else if (B % (seq_t)3 == (seq_t)0) {
        B /= (seq_t)3;
    } else {
        C /= (seq_t)3;
    }

    /* Multiply the three reduced factors */
    seq_t result = A * B;
    return result * C;
}
