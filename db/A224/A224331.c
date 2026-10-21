/*
 * A224331  Number of idempotent n X n 0..6 matrices of rank n-1.
 * https://oeis.org/A224331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*7^(n-1)-1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n < 1, return 0 */
    /* fast exponentiation: compute 7^(n-1) */
    seq_t power = (seq_t)1;
    seq_t base  = (seq_t)7;
    idx_t exp   = n - 1;

    while (exp)
    {
        if (exp & 1ULL) power *= base;    /* power *= base */
        base *= base;                     /* base *= base   */
        exp >>= 1;
    }

    /* term = 2*7^(n-1) - 1 */
    seq_t term = (seq_t)2 * power - (seq_t)1;
    /* result = n * term */
    return (seq_t)n * term;
}
