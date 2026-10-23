/*
 * A144535  Numerators of continued fraction convergents to sqrt(3)/2.
 * https://oeis.org/A144535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 14*a(n-2) - a(n-4) with a(0)=0, a(1)=1, a(2)=6, a(3)=13
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)0;
    if (n == 1u) return (seq_t)1;
    if (n == 2u) return (seq_t)6;
    if (n == 3u) return (seq_t)13;

    seq_t buf[4] = {(seq_t)0, (seq_t)1, (seq_t)6, (seq_t)13}; /* a0,a1,a2,a3 */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t new_val = buf[2] * (seq_t)14 - buf[0];
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = new_val;
    }
    return buf[3];
}
