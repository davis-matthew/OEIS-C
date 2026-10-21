/*
 * A086341  a(n) = 2*2^floor(n/2) - (-1)^n.
 * https://oeis.org/A086341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 62, u64 n <= 126,
 *            u128 n <= 254, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2*2^floor(n/2) - (-1)^n
 *  a(0) = 1, a(1) = 3, a(2) = 3, ...
 */
seq_t a(idx_t n)
{
    /* Compute 2^{floor(n/2)+1} */
    idx_t shift = (n >> 1) + 1;
    seq_t val = (seq_t)1;
    for (idx_t i = 0; i < shift; ++i)
        val *= (seq_t)2;

    /* Subtract (-1)^n : subtract 1 if n is even, add 1 if n is odd */
    if ((n & (idx_t)1) == 0)
        val -= (seq_t)1;
    else
        val += (seq_t)1;

    return val;
}
