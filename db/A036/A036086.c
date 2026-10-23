/*
 * A036086  Centered cube numbers: a(n) = (n+1)^8+n^8.
 * https://oeis.org/A036086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^8 + n^8
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 14, u64 n <= 234,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t _pow8(seq_t x)
{
    seq_t x2 = x * x;   /* x^2 */
    seq_t x4 = x2 * x2; /* x^4 */
    seq_t x8 = x4 * x4; /* x^8 */
    return x8;
}

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* cast index to seq_t */
    seq_t n1 = (seq_t)(n + 1);    /* (n+1) as seq_t */
    return _pow8(nn) + _pow8(n1);
}
