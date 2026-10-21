/*
 * A083233  a(n) = (3*8^n + 0^n)/4.
 * https://oeis.org/A083233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (3*8^n + 0^n)/4
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) = 1 (by 0^0 = 1). For n ≥ 1, 0^n = 0, so
       a(n) = 3*8^n/4 = 6*8^(n-1). 8^k = 2^(3k).  */
    if (n == 0U)
        return (seq_t)1;

    seq_t res = (seq_t)6;
    unsigned int shift = (unsigned int)(3 * (n - 1U));
    res <<= shift;           /* res = 6 * 2^(3*(n-1)) = 6*8^(n-1) */
    return res;
}
