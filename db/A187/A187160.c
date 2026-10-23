/*
 * A187160  Number of 7-step one space at a time bishop's tours on an n X n
 *          board summed over all starting positions.
 * https://oeis.org/A187160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 780*n*n - 6296*n + 12024 for n > 5 a(1) = 0, a(2) = 0,
 *            a(3) = 0, a(4) = 16, a(5) = 544 The empirical quadratic formula
 *            holds for all n ≥ 6
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n) {
    case 1:
    case 2:
    case 3:
        return (seq_t)0;
    case 4:
        return (seq_t)16;
    case 5:
        return (seq_t)544;
    default:
        seq_t nn = (seq_t)n;
        return ((seq_t)780 * nn * nn) - ((seq_t)6296 * nn) + (seq_t)12024;
    }
}
