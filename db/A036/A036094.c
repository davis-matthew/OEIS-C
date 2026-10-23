/*
 * A036094  Centered cube numbers: (n+1)^16 + n^16.
 * https://oeis.org/A036094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^16 + n^16
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 14, u128 n <= 244,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t pow16(seq_t x)
{
    seq_t r = 1;
    for (unsigned i = 0; i < 16; ++i)
        r *= x;
    return r;
}

seq_t a(idx_t n)
{
    seq_t n_seq        = (seq_t)n;
    seq_t n_plus_one   = n_seq + 1U;
    seq_t pow_n        = pow16(n_seq);
    seq_t pow_n1       = pow16(n_plus_one);
    return pow_n1 + pow_n;
}
