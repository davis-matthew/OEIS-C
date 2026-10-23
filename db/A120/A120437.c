/*
 * A120437  Differences of A037314 (sum of base-3 digits of n = sum of base-9
 *          digits of n).
 * https://oeis.org/A120437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is defined recursively * a(1) = 1 * a(3^k) = 9*a(3^(k-1)) -
 *            2 (k > 0) * a(n) = a(n - 3^(⌊log_3 n⌋)) (otherwise)
 * Range:     u8 n <= 26, u16 n <= 728, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base case, n must be ≥ 1 */
    if (n == 1)
        return (seq_t)1;

    /* Find the largest power of 3 that is ≤ n
     *  p = 3^k  with p ≤ n < 3·p
     */
    idx_t p = 1;
    while (p <= n / 3) /* avoid overflow */
        p *= 3;

    /* If n is itself a power of 3, apply the
     * 9-2 recurrence.
     */
    if (n == p) {
        seq_t sub = a(p / 3);        /* recursive call for smaller power */
        return (seq_t)9 * sub - (seq_t)2;
    }

    /* Otherwise subtract the largest lower power of 3 and recurse. */
    return a(n - p);
}
