/*
 * A096886  Expansion of (1+3*x)/(1-8*x^2).
 * https://oeis.org/A096886
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8^⌊n/2⌋ * (n even ? 1 : 3)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute k = floor(n/2) */
    idx_t k = n >> 1;              /* unsigned arithmetic */
    seq_t pow = 1;                 /* 8^k */
    seq_t base = 8;                /* 8^1 */

    /* Binary exponentiation for 8^k */
    while (k)
    {
        if (k & 1)
            pow *= base;           /* multiply only when needed */
        base *= base;              /* square the base */
        k >>= 1;
    }

    /* Multiply by 3 if n is odd */
    if (n & 1)
        pow *= 3;

    return pow;
}
