/*
 * A135545  a(n) = the largest proper divisor k of n where (k-1) and/or (k+1)
 *          divides (n+1).
 * https://oeis.org/A135545
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the largest proper divisor k of n such that (k-1)|(n+1)
 *            or (k+1)|(n+1)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) /* offset functionality is irrelevant below n = 3 */
        return (seq_t)0;

    idx_t best = 1;                 /* 1 always satisfies because 2 | (n+1) for any n */
    idx_t n_plus1 = n + 1;

    /* Check divisors up to sqrt(n).  No floating point used. */
    for (idx_t d = 2; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            idx_t d2 = n / d;

            /* First divisor d1 */
            if (d1 != n) {
                if ((d1 > 1 && n_plus1 % (d1 - 1) == 0) || n_plus1 % (d1 + 1) == 0) {
                    if (d1 > best) best = d1;
                }
            }

            /* Second divisor d2 (avoid duplicates) */
            if (d2 != d1 && d2 != n) {
                if ((d2 > 1 && n_plus1 % (d2 - 1) == 0) || n_plus1 % (d2 + 1) == 0) {
                    if (d2 > best) best = d2;
                }
            }
        }
    }

    return (seq_t)best;
}
