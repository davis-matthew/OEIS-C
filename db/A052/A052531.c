/*
 * A052531  If n is even then 2^n+1 otherwise 2^n.
 * https://oeis.org/A052531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A052531: a(n) = 2^n + (1+(-1)^n)/2 */
seq_t a(idx_t n)
{
    /* compute 2^n */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow2 <<= 1;            /* safe for unsigned types, wraps on overflow */

    /* if n even add 1 */
    if ((n & 1) == 0)
        pow2 += 1;

    return pow2;
}
