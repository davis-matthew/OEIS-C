/*
 * A042732  Numerators of continued fraction convergents to sqrt(896).
 * https://oeis.org/A042732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   * a(n) = 898*a(n-4) - a(n-8) for n >= 8 * a(0) = 29, a(1) = 30,
 *            a(2) = 419, a(3) = 449, * a(4) = 26461, a(5) = 26910, a(6) =
 *            376291, a(7) = 403201
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial terms of the sequence */
    static const seq_t init[8] = {
        29, 30, 419, 449,
        26461, 26910, 376291, 403201
    };

    /* For the first 8 indices, return the pre‑computed values */
    if (n < 8)
        return init[n];

    /* Sliding window of the last 8 computed terms.
     * The array index is always taken modulo 8. */
    seq_t h[8];
    for (idx_t i = 0; i < 8; ++i)
        h[i] = init[i];

    /* Compute from a(8) up to a(n) inclusive */
    for (idx_t k = 8; k <= n; ++k) {
        seq_t val = (seq_t)898 * h[(k - 4) % 8] - h[(k - 8) % 8];
        h[k % 8] = val;
    }

    return h[n % 8];
}
