/*
 * A134375  a(n) = (n!)^4.
 * https://oeis.org/A134375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n!)^4
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If n == 0, 0! = 1; 1^4 = 1. */
    if (n == 0)
        return (seq_t)1;

    seq_t result = (seq_t)1;

    /* Compute product_{k=1}^{n} k^4 directly.
       This yields (n!)^4 while keeping intermediate values as small
       as the final result requires. */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t p = (seq_t)k;
        p *= (seq_t)k;   /* p = k^2   */
        p *= (seq_t)k;   /* p = k^3   */
        p *= (seq_t)k;   /* p = k^4   */
        result *= p;
    }

    return result;
}
