/*
 * A104676  a(n) = binomial(n+2,2) * binomial(n+7,2).
 * https://oeis.org/A104676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+2,2) * binomial(n+7,2)
 * Range:     u8 n <= 2, u16 n <= 18, u32 n <= 358, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t1 = (nn + 2) * (nn + 1) / 2;   /* C(n+2,2) */
    seq_t t2 = (nn + 7) * (nn + 6) / 2;   /* C(n+7,2) */
    return t1 * t2;
}
