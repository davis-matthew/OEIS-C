/*
 * A107843  Number of iterations of McCarthy 91 Function until it terminates.
 * https://oeis.org/A107843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 203 - 2*n for n<101, a(n)=1 for n>100
 * Range:     u8 n <= 51, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 101) {
        return (seq_t)203 - ((seq_t)2 * n);
    } else {
        return (seq_t)1;
    }
}
