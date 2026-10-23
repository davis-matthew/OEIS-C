/*
 * A198473  Duplicate of A026185.
 * https://oeis.org/A198473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(2k) = 4k a(2k+1) = k+1 + floor(k/3) (k = (n-1)/2)
 * Range:     i8 n <= 63, i16 n <= 64, i32 n <= 64, i64 n <= 64, i128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {            /* n is even: n = 2k */
        seq_t k = (seq_t)n / 2;
        return k * 4;
    } else {                        /* n is odd: n = 2k+1 */
        seq_t k = ((seq_t)n - 1) / 2;
        seq_t result = (k + 1) + (k / 3);
        return result;
    }
}
