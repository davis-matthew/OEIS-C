/*
 * A192004  Alternating row sums of array A187360: minimal polynomial of
 *          2*cos(Pi/n) evaluated for x=-1.
 * https://oeis.org/A192004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 220, i16 n <= 220, i32 n <= 220, i64 n <= 220,
 *            i128 n <= 220, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = -2 if n divisible by 3 and n/3 is a power of two,
 *        -1 if n is a power of two (and n > 1),
 *        1 otherwise.
 * Note that a(1) = 1.
 */
seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;

    if ((n % (idx_t)3) == (idx_t)0) {
        idx_t m = n / (idx_t)3;
        if (m && ((m & (m - (idx_t)1)) == (idx_t)0))
            return (seq_t)-2;
    }

    if ((n & (n - (idx_t)1)) == (idx_t)0) /* power of two */
        return (seq_t)-1;

    return (seq_t)1;
}
