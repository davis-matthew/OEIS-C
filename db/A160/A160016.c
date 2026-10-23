/*
 * A160016  Lodumo_2 transform of A159833.
 * https://oeis.org/A160016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-4) + [6, 6, 2, 6] for n >= 4, with a(0) = 0, a(1) =
 *            2, a(2) = 1, a(3) = 4
 * Range:     u8 n <= 170, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[4] = { (seq_t)0, (seq_t)2, (seq_t)1, (seq_t)4 };

    if (n < 4)
        return init[n];

    seq_t buf[4] = { (seq_t)0, (seq_t)2, (seq_t)1, (seq_t)4 };

    for (idx_t i = 4; i <= n; ++i) {
        idx_t r = i & 3;                 /* same as i % 4 */
        buf[r] += (r == 2 ? (seq_t)2 : (seq_t)6);
    }

    return buf[n & 3];
}
