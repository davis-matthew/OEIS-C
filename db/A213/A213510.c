/*
 * A213510  The sequence N(n) arising in the enumeration of balanced ternary
 *          trees.
 * https://oeis.org/A213510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + sum_{i=2}^{n} (floor(log_3(2*i-1)) + 1)
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined recursively by: 
 *   a(1) = 1,
 *   a(n) = a(n-1) + s(n),  for n > 1,
 *   where s(n) = floor(log_3(2*n-1)) + 1.
 * This corresponds to incremental additions of 2, 3, 4, ... each for
 * 3, 9, 27, ... consecutive indices, respectively.
 */

seq_t a(idx_t n) {
    if (n <= 1) {
        /* For n = 0 the sequence is not defined; return 0 to avoid
         * underflow when called with n=0.  This behaviour has no
         * effect for valid OEIS indices (n >= 1). */
        return (seq_t)n;
    }

    seq_t result = 1;            /* a(1) */
    seq_t s = 2;                 /* current increment */
    seq_t next_power = 9;        /* 3^s, threshold where s increases */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t val = (seq_t)2 * (seq_t)i - 1;   /* 2*i - 1 */
        if (val >= next_power) {
            ++s;
            next_power *= 3;   /* advance next threshold: 3^(s+1) */
        }
        result += s;
    }
    return result;
}
