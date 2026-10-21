/*
 * A086949  Numbers k such that R(k+9) = 5.
 * https://oeis.org/A086949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 41, a(2) = 491, a(n) = 11*a(n-1) - 10*a(n-2) for n > 2
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence for n ≥ 1 is a(n) = 5*10^n - 9. */

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* not part of the sequence, keep safe */
    if (n == 1) return (seq_t)41;    /* a(1) */
    if (n == 2) return (seq_t)491;   /* a(2) */

    seq_t prev2 = (seq_t)41;         /* a(1)   */
    seq_t prev1 = (seq_t)491;        /* a(2)   */
    seq_t curr  = 0;

    for (idx_t i = 3; i <= n; ++i) {
        curr = (seq_t)11 * prev1 - (seq_t)10 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
