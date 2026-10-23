/*
 * A156202  a(n) = 2*a(n-1)+3 for n > 1, a(1) = 10.
 * https://oeis.org/A156202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -3 + 13*2^(n-1)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) = 10, a(n) = 2*a(n-1) + 3 for n > 1 */
    if (n == 0)  /* offset not used; return 0 for safety */
        return 0;

    seq_t pow2 = (seq_t)1;   /* pow2 = 2^(n-1) */
    for (idx_t i = n - 1; i > 0; --i)
        pow2 *= (seq_t)2;    /* iterative doubling avoids UB for large shifts */

    seq_t result = pow2 * (seq_t)13; /* 13 * 2^(n-1) */
    result -= (seq_t)3;             /* subtract 3 */
    return result;
}
