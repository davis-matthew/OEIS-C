/*
 * A058319  Coefficients (multiplied by 48) in Alternative Extended Simpson's
 *          rule for numerical integration.
 * https://oeis.org/A058319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 48 for n ≥ 4; a(n) for 0 ≤ n ≤ 3 is the unique cubic
 *            interpolant of the first four terms. a(n) = (40⋅n³ – 207⋅n² +
 *            293⋅n + 51) / 3
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4) {
        seq_t nn = (seq_t)n;
        /* Compute the cubic polynomial without overflow for n<4 */
        seq_t val = 40 * nn * nn * nn
                  - 207 * nn * nn
                  + 293 * nn
                  + 51;
        return val / 3;
    }
    return 48;
}
