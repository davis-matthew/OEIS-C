/*
 * A131290  1 followed by period 6: repeat [3, 2, 0, -1, 0, 2].
 * https://oeis.org/A131290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1; for n ≥ 1, a(n) = {3, 2, 0, -1, 0, 2}[(n-1) mod 6]
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: uses a periodic pattern after the initial term. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    static const seq_t pattern[6] = {
        (seq_t) 3, (seq_t) 2, (seq_t) 0,
        (seq_t)-1, (seq_t) 0, (seq_t) 2
    };
    idx_t r = (n - 1) % 6;
    return pattern[r];
}
