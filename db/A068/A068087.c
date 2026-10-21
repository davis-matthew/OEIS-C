/*
 * A068087  a(n) = n^(2*n-2).
 * https://oeis.org/A068087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^(2*n-2)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Returning the value of n raised to the power (2*n-2).
       The exponentiation is performed by repeated squaring. */
    if (n == 0)         /* not part of the OEIS offset but safe guard */
        return 0;
    seq_t result = 1;
    seq_t base   = (seq_t)n;
    idx_t  exp   = 2 * n - 2;   /* exponent is (2*n-2) */

    while (exp) {
        if (exp & 1UL)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}
