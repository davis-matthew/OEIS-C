/*
 * A085525  a(n) = n^(2*n + 2).
 * https://oeis.org/A085525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(2*n + 2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* (seq_t)n is the base, exponent is 2*n+2 */
    seq_t base   = (seq_t)n;
    idx_t exp    = 2*n + 2;
    seq_t result = (seq_t)1;

    while (exp > 0) {
        if (exp & 1) {            /* if lowest bit of exp is set */
            result = result * base;
        }
        base = base * base;       /* square the base */
        exp >>= 1;                /* shift exponent right */
    }
    return result;
}
