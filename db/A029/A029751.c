/*
 * A029751  Average theta series of odd unimodular lattices in dimension 12.
 * https://oeis.org/A029751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 8 * Σ_{d|n} d^5 * ((n/d)&1 ? 1 : -1) * (with a(0) = 1)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 56, u64 n <= 4704,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)                     /* a(0) = 1 */
        return 1;

    seq_t result = 0;                /* accumulates the sum */

    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d) continue;         /* skip non‑divisors */

        idx_t e = n / d;             /* paired divisor */

        /* compute d^5 */
        seq_t d5 = (seq_t)d;
        d5 *= d5;  /* d^2 */
        d5 *= d5;  /* d^4 */
        d5 *= (seq_t)d;  /* d^5 */

        /* compute e^5 only if e != d (to avoid double work) */
        seq_t e5 = (seq_t)e;
        e5 *= e5;
        e5 *= e5;
        e5 *= (seq_t)e;               /* e^5 */

        if (d == e) {                 /* perfect square divisor */
            if (e & 1) {              /* (n/d) odd → positive sign */
                result += (d5 << 3);  /* 8 * d^5 */
            } else {
                result -= (d5 << 3);  /* negative sign */
            }
        } else {
            /* contribution from divisor d: sign determined by (e) */
            if (e & 1) result += (d5 << 3);
            else       result -= (d5 << 3);

            /* contribution from divisor e: sign determined by (d) */
            if (d & 1) result += (e5 << 3);
            else       result -= (e5 << 3);
        }
    }

    return result;
}
