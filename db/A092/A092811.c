/*
 * A092811  Expansion of g.f. (1-4*x)/(1-8*x).
 * https://oeis.org/A092811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*8^(n-1) for n > 0, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;          /* a(0) = 1 */

    seq_t val = (seq_t)1;

    /* Multiply by 8 (n–1) times: val = 8^(n-1) */
    for (idx_t i = (idx_t)1; i < n; ++i)
        val *= (seq_t)8;

    /* Finally multiply by 4: a(n) = 4*8^(n-1) */
    val *= (seq_t)4;
    return val;
}
