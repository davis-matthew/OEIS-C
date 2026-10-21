/*
 * A080141  a(n) = (3^(n-1))*(n!)^2.
 * https://oeis.org/A080141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^(n-1)) * (n!)^2 Recurrence used: a(1) = 1 a(n) =
 *            a(n-1) * 3 * n * n (for n >= 2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* Offset starts at 1; 0 is undefined */
    seq_t result = (seq_t)1;        /* a(1) = 1 */

    for (idx_t i = 2; i <= n; ++i) {
        /* Compute factor = 3 * i * i */
        seq_t factor = (seq_t)i * (seq_t)i;   /* i^2 as seq_t */
        factor *= (seq_t)3;                   /* multiply by 3 */
        result *= factor;                     /* accumulate */
    }
    return result;
}
