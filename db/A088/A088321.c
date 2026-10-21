/*
 * A088321  Duplicate of A061301.
 * https://oeis.org/A088321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 2^(n*2^(n-1))
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 3, i64 n <= 4, i128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial case n = 0 */
    if (n == 0)
        return (seq_t)1;

    /* Compute 2^(n-1) with a simple left‑shift loop. */
    seq_t two_pow = (seq_t)1;
    for (idx_t i = 0; i < n - 1; ++i)
        two_pow = two_pow * (seq_t)2;

    /* Exponent e = n * 2^(n-1) */
    seq_t exp = (seq_t)n * two_pow;

    /* Compute 2^exp by binary exponentiation. */
    seq_t base   = (seq_t)2;
    seq_t result = (seq_t)1;
    seq_t e      = exp;
    while (e > 0) {
        if (e & (seq_t)1)
            result = result * base;
        base = base * base;
        e    = e >> 1;
    }
    return result;
}
