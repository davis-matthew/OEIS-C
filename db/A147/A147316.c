/*
 * A147316  Fibonacci numbers (A000045) starting at offset -20.
 * https://oeis.org/A147316
 *
 * Offset:    -20    (a(-20) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = a(n-1) + a(n-2); a(0) = 0, a(1) = 1
 * Range:     i8 none, i16 n <= 23, i32 n <= 46, i64 n <= 92, i128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fibonacci numbers extended to negative indices: F(-n) = (-1)^(n+1) F(n). */

seq_t a(idx_t n)
{
    /* Trivial cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    idx_t m = n;
    if (m < 0) m = -m;          /* absolute value */

    seq_t f0 = (seq_t)0;
    seq_t f1 = (seq_t)1;
    seq_t tmp;

    for (idx_t i = 2; i <= m; ++i) {
        tmp = f0 + f1;
        f0 = f1;
        f1 = tmp;
    }

    seq_t val = f1;

    /* Negate if required for negative indices */
    if (n < 0 && ((m & 1) == 0)) {
        val = -val;
    }
    return val;
}
