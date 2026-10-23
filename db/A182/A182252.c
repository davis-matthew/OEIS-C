/*
 * A182252  a(0)=4, a(n) = (a(n-1) OR n) * n.
 * https://oeis.org/A182252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = (a(n‑1) OR n) * n, a(0) = 4
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)4;          /* a(0) */
    if (n == 0)
        return val;

    for (idx_t i = 1; i <= n; ++i)
        val = (val | (seq_t)i) * (seq_t)i;

    return val;
}
