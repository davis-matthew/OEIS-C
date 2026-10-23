/*
 * A040902  Continued fraction for sqrt(933).
 * https://oeis.org/A040902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=30; for n>=1, a(n) = period[(n-1)%8] where period =
 *            [1,1,5,20,5,1,1,60]
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[8] = {1, 1, 5, 20, 5, 1, 1, 60};
    if (n == 0)
        return (seq_t)30;
    /* (n-1)&7 computes (n-1)%8 since 8 is a power of 2 */
    const idx_t k = (n - 1) & 7;
    return period[k];
}
