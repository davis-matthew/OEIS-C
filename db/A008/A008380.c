/*
 * A008380  4*(2n-1)!*H(2n), where H(n) = Sum 1/i are harmonic numbers.
 * https://oeis.org/A008380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*(2n-1)!*H(2n) where H(m) = Sum_{i=1}^m 1/i
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence starts at index 1 */
    if (n < 1) return (seq_t)0;
    if (n == 1) return (seq_t)6;            /* a(1) = 6 */

    seq_t a_val = (seq_t)6;                  /* current a(n) value */
    seq_t fac   = (seq_t)1;                  /* (2*1-1)! = 1! */

    for (idx_t k = 2; k <= n; ++k) {
        /* mult = (2k-1)*(2k-2) */
        seq_t mult = ((seq_t)k * 2) - 1;     /* 2k-1 */
        mult *= ((seq_t)k * 2) - 2;          /* times (2k-2) */

        /* term2 = fac * (k-1) * (4k-1) * 4 / k  (exact division) */
        seq_t term2 = fac;
        term2 *= (seq_t)(k - 1);
        term2 *= (seq_t)(4 * k - 1);
        term2 *= (seq_t)4;
        term2 /= (seq_t)k;

        /* Update factorial and value for next iteration */
        fac   *= mult;
        a_val *= mult;
        a_val += term2;
    }

    return a_val;
}
