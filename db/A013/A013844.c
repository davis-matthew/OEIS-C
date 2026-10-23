/*
 * A013844  a(n) = 7^(5*n + 3).
 * https://oeis.org/A013844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^(5*n + 3)
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 3, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Exponent: 5*n + 3, fits in idx_t for n up to 10000 */
    idx_t exp = (idx_t)5 * n + (idx_t)3;
    seq_t base = (seq_t)7;
    seq_t result = (seq_t)1;

    /* binary exponentiation */
    while (exp > 0) {
        if (exp & 1ULL)
            result *= base;
        exp >>= 1;
        if (exp)           /* avoid useless squaring when exp==0 */
            base *= base;
    }
    return result;
}
