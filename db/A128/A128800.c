/*
 * A128800  a(n) = n*(n-1)*6^n.
 * https://oeis.org/A128800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*6^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 21, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)          /* a(0) = 0, a(1) = 0 */
        return 0;

    /* binary exponentiation: compute 6^n */
    idx_t e = n;
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)6;

    while (e) {
        if (e & 1)          /* odd exponent: multiply result by base */
            result *= base;
        base *= base;       /* square base */
        e >>= 1;            /* floor(e / 2) */
    }

    /* multiply by n*(n-1) */
    seq_t coeff = (seq_t)n * (seq_t)(n - 1);
    return result * coeff;
}
