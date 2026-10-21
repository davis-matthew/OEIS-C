/*
 * A074150  Duplicate of A061925.
 * https://oeis.org/A074150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(2t) = 2*t^2 + 1 a(2t+1) = 2*t*(t+1) + 2 (equivalently a(n) =
 *            a(n-1) + 2*floor((n-1)/2) + 1 with a(0)=1)
 * Range:     i8 n <= 15, i16 n <= 21, i32 n <= 21, i64 n <= 21, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* integer division truncates toward zero; n/2 is floor(n/2) */
    seq_t t = (seq_t)(n / 2);
    if ((n & 1) == 0) {                /* n is even: n = 2t       */
        return (2 * t * t) + 1;
    } else {                            /* n is odd:  n = 2t + 1   */
        return (2 * t * (t + 1)) + 2;
    }
}
