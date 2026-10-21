/*
 * A084259  a(n) = n! * (n+1)! * (n+2)! * (n+3).
 * https://oeis.org/A084259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (n+1)! * (n+2)! * (n+3)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n! */
    seq_t fac = 1;
    for (idx_t i = 1; i <= n; ++i) {
        fac *= (seq_t)i;
    }

    /* compute (n!)^3 */
    seq_t res = fac * fac;
    res *= fac;

    /* multiply by (n+1)^2 * (n+2) * (n+3) */
    res *= (seq_t)(n + 1);
    res *= (seq_t)(n + 1);
    res *= (seq_t)(n + 2);
    res *= (seq_t)(n + 3);

    return res;
}
