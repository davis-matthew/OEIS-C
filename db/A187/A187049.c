/*
 * A187049  Number of 5-step one or two space at a time bishop's tours on an n
 *          X n board summed over all starting positions.
 * https://oeis.org/A187049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2540*n^2 - 21128*n + 43936 for n >= 8; Small n: a(1)=0,
 *            a(2)=0, a(3)=8, a(4)=456, a(5)=2896, a(6)=9216, a(7)=20648
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t small[] = {0, 0, 8, 456, 2896, 9216, 20648};
    if (n <= 7) {
        return small[n - 1];
    }
    seq_t nn = (seq_t)n;
    seq_t result = (seq_t)2540 * nn * nn - (seq_t)21128 * nn + (seq_t)43936;
    return result;
}
