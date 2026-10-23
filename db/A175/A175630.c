/*
 * A175630  a(n) = n-th pentagonal number mod (n+2).
 * https://oeis.org/A175630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n*(3*n-1)/2) mod (n+2)
 * Range:     u8 n <= 9, u16 n <= 174, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n == 0 the pentagonal number is 0 and 0 mod 2 == 0 */
    if (n == 0)
        return 0;

    seq_t ns = (seq_t)n;          /* Convert the index to seq_t */
    seq_t t  = 3 * ns - 1;        /* Compute 3*n - 1 */
    seq_t num = ns * t;           /* Compute n*(3*n-1) */
    seq_t pent = num / 2;         /* Divide by 2 (exact division) */
    seq_t mod  = ns + 2;          /* Compute n+2 for the modulus */
    return pent % mod;            /* Result of the modulo operation */
}
