/*
 * A009775  Exponential generating function is tanh(log(1+x)).
 * https://oeis.org/A009775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 0 if n == 0 or n % 4 == 3 * = (-1)^(n % 8 ∈ {0,2,5}) * n!
 *            / 2^{⌊n/2⌋} otherwise
 * Range:     i8 n <= 7, i16 n <= 9, i32 n <= 15, i64 n <= 23, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* offset zero */
    if (n == 0)
        return 0;

    /* a(4k+3) = 0 */
    if ((n & 3) == 3)
        return 0;

    /* Compute n! / 2^⌊n/2⌋ without overflow */
    idx_t need = n >> 1;        /* number of 2's to strip */
    seq_t res = 1;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t f = (seq_t)i;
        while (need > 0 && (f & 1) == 0) {
            f >>= 1;
            --need;
        }
        res *= f;
    }

    /* Apply sign according to n mod 8 */
    idx_t r = n & 7;
    if (r == 0 || r == 2 || r == 5)
        res = -res;

    return res;
}
