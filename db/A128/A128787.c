/*
 * A128787  a(n) = n^2*8^n.
 * https://oeis.org/A128787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*8^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* a(0) = 0 */

    /* compute 8^n by binary exponentiation */
    seq_t pow8 = 1;
    seq_t base = (seq_t)8;
    idx_t  exp  = n;
    while (exp) {
        if (exp & 1) pow8 *= base;
        base *= base;
        exp >>= 1;
    }

    seq_t n_sq = (seq_t)n * (seq_t)n;
    return n_sq * pow8;
}
