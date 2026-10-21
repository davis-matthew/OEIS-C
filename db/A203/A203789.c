/*
 * A203789  Number of (n+1)X2 0..3 arrays with every 2X2 subblock having equal
 *          diagonal elements or equal antidiagonal elements.
 * https://oeis.org/A203789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*7^n (offset 1)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res    = (seq_t)1;
    seq_t base   = (seq_t)7;
    idx_t exp = n;

    /* exponentiation by squaring, using only allowed operators */
    while (exp != 0) {
        if (exp & 1)
            res *= base;
        base *= base;
        exp >>= 1;
    }

    return ((seq_t)16) * res;   /* final result */
}
