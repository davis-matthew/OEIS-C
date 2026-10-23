/*
 * A154352  a(n)=A132658(n+1)-2*A132658(n).
 * https://oeis.org/A154352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A132658(n+1) - 2*A132658(n)
 * Range:     i8 n <= 12, i16 n <= 18, i32 n <= 36, i64 n <= 42, i128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Zero for multiples of 3 */
    if ((n % 3) == 0) return 0;

    /* m = floor(n/3) */
    idx_t m = n / 3;

    /* Compute magnitude */
    seq_t mag;
    if (m < 2) {
        mag = 1;
    } else {
        idx_t j = (m - 2) / 2;      /* exponent of 43 */
        mag = 21;
        for (idx_t i = 0; i < j; ++i) {
            mag *= (seq_t)43;
        }
    }

    /* Determine sign */
    if (m == 0) {
        return mag;           /* positive for n=1,2 */
    } else if (m == 1) {
        return -mag;          /* negative for n=4,5 */
    } else {
        /* For m >= 2: sign = (-1)^(m-1) */
        return (m % 2 == 0) ? -mag : mag;
    }
}
