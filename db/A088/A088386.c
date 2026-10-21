/*
 * A088386  a(n) = 2^n*(n!)^3.
 * https://oeis.org/A088386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n * (n!)^3
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)  = 1
 *             a(n) = 2 * n^3 * a(n-1)  for n >= 1
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;

    for (idx_t i = (idx_t)1; i <= n; ++i) {
        /* multiply by 2^n part */
        result *= (seq_t)2;

        /* multiply by i^3 part (carry on factorial cubed) */
        seq_t i3 = (seq_t)i;
        i3 *= (seq_t)i;
        i3 *= (seq_t)i;
        result *= i3;
    }

    return result;
}
