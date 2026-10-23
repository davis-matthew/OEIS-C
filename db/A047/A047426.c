/*
 * A047426  Numbers that are congruent to {0, 3, 4, 5, 6} mod 8.
 * https://oeis.org/A047426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/5) + [0, 3, 4, 5, 6][(n-1)%5]
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n ≥ 1 by definition of the sequence.  For n == 0 the
       function returns 0, but such a call is outside the
       specification and ignored. */
    idx_t q = (n - 1) / 5;            /* quotient: number of full blocks of 5 */
    idx_t r = (n - 1) % 5;            /* remainder: 0 .. 4 */

    static const seq_t offsets[5] = { 0, 3, 4, 5, 6 };

    /* Multiplication and addition are performed in seq_t to avoid
       intermediate overflows for the various integer sizes
       supported by the build.  The cast to seq_t is safe because
       q fits in idx_t, which is unsigned and small enough for
       ordinary use. */
    return (seq_t)8 * (seq_t)q + offsets[r];
}
