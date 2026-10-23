/*
 * A162994  The third left hand column of triangle A162990.
 * https://oeis.org/A162994
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((n+1)!/3)^2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 3. For other indices return 0. */
    if (n < 3) {
        return 0;
    }

    seq_t f = 1;
    for (idx_t i = 2; i <= n + 1; ++i) {
        f *= (seq_t)i;
    }
    f /= (seq_t)3;           /* (n+1)! is always divisible by 3 for n ≥ 3 */
    return f * f;            /* square the result */
}
