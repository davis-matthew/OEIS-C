/*
 * A036092  Centered cube numbers: a(n) = (n+1)^14 + n^14.
 * https://oeis.org/A036092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^14 + n^14
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 22, u128 n <= 537,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t pow14(seq_t x)
{
    seq_t result = 1;
    for (idx_t i = 0; i < 14; ++i)
    {
        result *= x;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t sn  = (seq_t)n;
    seq_t sn1 = sn + 1;
    seq_t p_n  = pow14(sn);
    seq_t p_np1 = pow14(sn1);
    return p_n + p_np1;
}
