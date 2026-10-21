/*
 * A203927  Number of (n+1)X2 0..5 arrays with every 2X2 subblock having equal
 *          diagonal elements or equal antidiagonal elements.
 * https://oeis.org/A203927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 36 * 11^n
 * Range:     u8 none, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 1. Compute 36 * 11^n using binary exponentiation. */
    seq_t result = (seq_t)36;
    seq_t base   = (seq_t)11;
    idx_t exp    = n;

    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}
