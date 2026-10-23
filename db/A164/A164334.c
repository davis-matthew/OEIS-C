/*
 * A164334  Quartic recurrence sequence a(0) = 1, a(n) = n*a(n-1)^4.
 * https://oeis.org/A164334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(n) = n*a(n-1)^4
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = 1;
    for (idx_t i = 1; i <= n; ++i) {
        /* r = r^4 */
        seq_t r2 = r * r;
        seq_t r4 = r2 * r2;
        /* r = i * r^4 */
        r = (seq_t)i * r4;
    }
    return r;
}
