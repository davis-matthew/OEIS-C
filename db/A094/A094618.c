/*
 * A094618  a(n) = 3^(n+1) - 2^(n+1) + n + 1.
 * https://oeis.org/A094618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(n+1) - 2^(n+1) + n + 1 (offset 0)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i)
        res *= base;
    return res;
}

seq_t a(idx_t n)
{
    idx_t exp = n + 1;               /* compute 3^(n+1) and 2^(n+1) */
    seq_t pow3 = pow_seq_t((seq_t)3, exp);
    seq_t pow2 = pow_seq_t((seq_t)2, exp);
    return pow3 - pow2 + (seq_t)n + 1;
}
