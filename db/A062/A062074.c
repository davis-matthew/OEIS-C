/*
 * A062074  a(n) = n^3 * 3^n.
 * https://oeis.org/A062074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^3 * 3^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 31, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* fast exponentiation to compute 3^n */
    seq_t pow3   = 1;
    seq_t base   = 3;
    idx_t  exp   = n;
    while (exp != 0) {
        if (exp & 1)
            pow3 *= base;
        base *= base;
        exp >>= 1;
    }

    /* compute n^3 */
    seq_t n3 = (seq_t)n * (seq_t)n;
    n3     *= (seq_t)n;

    /* final result */
    return pow3 * n3;
}
