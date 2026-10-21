/*
 * A083392  Alternating partial sums of A000217.
 * https://oeis.org/A083392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^n * floor((n+1)^2 / 4)
 * Range:     i8 n <= 10, i16 n <= 201, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Another equivalent formula: a(n) = (-1)^n * ((n^2 + n)/2 - floor(n^2/4)) */
seq_t a(idx_t n)
{
    /* Compute (n+1)^2 */
    seq_t np1 = (seq_t)n + 1;
    seq_t sq  = np1 * np1;

    /* floor((n+1)^2 / 4) */
    seq_t val = sq / 4;

    /* Apply sign: (-1)^n */
    if ((n & 1ULL) != 0)
        val = -val;

    return val;
}
