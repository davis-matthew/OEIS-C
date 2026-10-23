/*
 * A014896  a(1) = 1, a(n) = 13*a(n-1) + n.
 * https://oeis.org/A014896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 13*a(n-1) + n (a(1) = 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)               /* a(1) = 1 */
        return (seq_t)1;

    seq_t val = (seq_t)1;     /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        /* val = 13 * val + i */
        val = val * (seq_t)13 + (seq_t)i;
    }
    return val;
}
