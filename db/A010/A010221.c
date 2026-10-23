/*
 * A010221  Continued fraction for sqrt(177).
 * https://oeis.org/A010221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 13; for n>=1, a(n) = period[(n-1) % 8] with period = [3,
 *            3, 2, 8, 2, 3, 3, 26] (the continued fraction of sqrt(177))
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[8] = {3, 3, 2, 8, 2, 3, 3, 26};
    if (n == 0) {
        return 13;
    } else {
        idx_t k = (n - 1) % 8;
        return period[k];
    }
}
