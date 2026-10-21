/*
 * A215456  a(n)=(4n-1)!! modulo 2n, n=1,2,...
 * https://oeis.org/A215456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4n-1)!! mod 2n, offset 1
 * Range:     u8 n <= 63, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (4n-1)!! is the double factorial of the odd number 4n-1 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* offset is 1, but guard against 0 */
    seq_t mod = (seq_t)(2 * n);     /* modulus 2n */
    seq_t res = 1;
    /* iterate over odd numbers from 1 to (4n-1) */
    for (seq_t i = 1; i <= (seq_t)(4 * n - 1); i += 2)
        res = (res * i) % mod;
    return res;
}
