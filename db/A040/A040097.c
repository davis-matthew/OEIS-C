/*
 * A040097  Duplicate of A010174.
 * https://oeis.org/A040097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 10 if n = 1; Otherwise, let r = n mod 8: r = 1 -> 20 r =
 *            0 or 2 -> 2 r = 3,4,6,7 -> 1 r = 5 -> 4
 * Range:     i8 n <= 25, i16 n <= 25, i32 n <= 25, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case for the first term */
    if (n == (idx_t)1) {
        return (seq_t)10;
    }

    /* Table indexed by n mod 8 */
    static const seq_t table[8] = {
        /* r = 0 */ (seq_t)2,
        /* r = 1 */ (seq_t)20,
        /* r = 2 */ (seq_t)2,
        /* r = 3 */ (seq_t)1,
        /* r = 4 */ (seq_t)1,
        /* r = 5 */ (seq_t)4,
        /* r = 6 */ (seq_t)1,
        /* r = 7 */ (seq_t)1
    };

    idx_t r = n % (idx_t)8;
    return table[r];
}
