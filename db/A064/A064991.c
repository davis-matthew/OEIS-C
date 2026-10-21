/*
 * A064991  In A064990, drop leading 0's from A_k (for k>0) and convert to
 *          decimal.
 * https://oeis.org/A064991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n+1) = a(n)(4^3^n + 2^3^n - 1) + 2^3^n - 1 Initial value: a(0)
 *            = 0
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t a_val = 0;          /* a(0) */
    if (n == 0)
        return a_val;

    seq_t t = 2;              /* t = 2^(3^0) */

    for (idx_t i = 0; i < n; ++i)
    {
        /* coefficient = t^2 + t - 1 */
        seq_t t2 = t * t;        /* t^2 = 2^(2*3^i)     */
        seq_t coeff = t2 + t;    /* t^2 + t             */
        coeff -= 1;              /* t^2 + t - 1         */

        seq_t new_a = a_val * coeff;   /* a * coeff           */
        new_a += t - 1;               /* + (t - 1)           */
        a_val = new_a;                /* a(n+1) */

        /* t for next iteration: t = t^3 = 2^(3^{i+1}) */
        seq_t t3 = t * t;
        t = t3 * t;
    }

    return a_val;
}
