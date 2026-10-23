/*
 * A036090  Centered cube numbers: (n+1)^12 + n^12.
 * https://oeis.org/A036090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)^12 + n^12
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 5, u64 n <= 37, u128 n <= 1533,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t pow12(seq_t x)
{
    seq_t r = (seq_t)1;
    for (int i = 0; i < 12; ++i)
        r *= x;
    return r;
}

seq_t a(idx_t n)
{
    seq_t ns   = (seq_t)n;
    seq_t np1  = ns + (seq_t)1;
    seq_t p_n  = pow12(ns);
    seq_t p_np1= pow12(np1);
    return p_np1 + p_n;
}
