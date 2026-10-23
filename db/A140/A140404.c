/*
 * A140404  a(n) = binomial(n+5, 5)*7^n.
 * https://oeis.org/A140404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+5,5) * 7^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* binomial(n+5,5) = (n+5)(n+4)(n+3)(n+2)(n+1)/120 */
    seq_t binom = ((nn + 5) * (nn + 4) * (nn + 3) * (nn + 2) * (nn + 1)) / 120;

    /* fast exponentiation for 7^n */
    seq_t pow7 = 1;
    seq_t base = 7;
    idx_t e = n;
    while (e) {
        if (e & 1) pow7 *= base;
        base *= base;
        e >>= 1;
    }

    return binom * pow7;
}
