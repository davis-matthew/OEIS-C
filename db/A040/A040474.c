/*
 * A040474  Continued fraction for sqrt(497).
 * https://oeis.org/A040474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(0)=22; for n≥1 a(n)=periodic[(n-1)%10] with period 10:
 *            {3,2,2,5,6,5,2,2,3,44}
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* periodic part of the continued fraction of sqrt(497) */
    static const seq_t period[10] = {3, 2, 2, 5, 6, 5, 2, 2, 3, 44};
    if (n == 0)
        return (seq_t)22;
    idx_t idx = (n - 1) % 10;
    return period[idx];
}
