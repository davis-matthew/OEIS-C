/*
 * A227624  a(n) = numerator of r(n), where r(n) = (4*n+2)!/((3*n)!*2^n).
 * https://oeis.org/A227624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = numerator of (4*n+2)! / ((3*n)! * 2^n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* product range: (3*n+1) * … * (4*n+2)   */
    idx_t start = 3 * n + 1;
    idx_t end   = 4 * n + 2;

    seq_t num = 1;   /* product of odd parts */
    idx_t  e   = 0;   /* total exponent of 2 in the product */

    for (idx_t k = start; k <= end; ++k) {
        seq_t part = (seq_t)k;
        idx_t v2   = 0;

        /* strip all 2‑factors from part */
        while ((part & 1) == 0) {
            part >>= 1;
            ++v2;
        }

        e   += v2;
        num *= part;
    }

    /* cancel as many 2^n as possible, keep any surplus 2's */
    if (e > n) {
        idx_t leftover = e - n;
        for (idx_t i = 0; i < leftover; ++i)
            num *= (seq_t)2;
    }

    return num;
}
