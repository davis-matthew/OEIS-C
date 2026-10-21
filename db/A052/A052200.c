/*
 * A052200  Number of n-state, 2-symbol, d+ in {LEFT, RIGHT}, 5-tuple (q, s,
 *          q+, s+, d+) (halting or not) Turing machines.
 * https://oeis.org/A052200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*(n+1))^(2*n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base = 4*(n+1) as seq_t */
    seq_t base = ((seq_t)(n + 1)) * ((seq_t)4);
    /* exponent = 2*n as idx_t */
    idx_t exp = (idx_t)(2 * n);
    seq_t result = (seq_t)1;

    while (exp > 0) {
        if (exp & 1) {
            result *= base;
        }
        exp >>= 1;
        if (exp) {          /* no need to square on last iteration */
            base *= base;
        }
    }
    return result;
}
