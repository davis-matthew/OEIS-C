/*
 * A060851  a(n) = (2n-1) * 3^(2n-1).
 * https://oeis.org/A060851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n-1)*3^(2*n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    idx_t exp = 2*n - 1;             /* exponent for 3^exp */
    seq_t result = 1;
    seq_t base   = 3;

    /* fast exponentiation: result = 3^exp */
    while (exp > 0)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }

    /* multiply by the linear factor (2*n-1) */
    result *= (seq_t)(2*n - 1);
    return result;
}
