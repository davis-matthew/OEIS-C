/*
 * A040896  Continued fraction for sqrt(927).
 * https://oeis.org/A040896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=⌊√927⌋=30; for n≥1 the terms repeat with period 8:
 *            (2,4,5,3,5,4,2,60)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)30;
    static const seq_t period[8] = {
        (seq_t)2, (seq_t)4, (seq_t)5, (seq_t)3,
        (seq_t)5, (seq_t)4, (seq_t)2, (seq_t)60
    };
    /* n-1 is safe because n≥1 */
    return period[(n - 1) % 8];
}
