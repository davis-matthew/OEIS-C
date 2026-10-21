/*
 * A080925  Binomial transform of Jacobsthal gap sequence (A080924).
 * https://oeis.org/A080925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (3^n + (-1)^n)/2 for n ≥ 1, a(0) = 0 The recurrence a(n)
 *            = 3*a(n-1) + 2*(-1)^n is used to avoid intermediate overflow
 *            when the result fits in seq_t
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }

    /* a(1) = 1 */
    seq_t term = (seq_t)1;
    if (n == 1) {
        return term;
    }

    for (idx_t i = 2; i <= n; ++i) {
        if (i & 1) {                 /* i is odd: sign is -1 */
            term = term * (seq_t)3 - (seq_t)2;
        } else {                     /* i is even: sign is +1 */
            term = term * (seq_t)3 + (seq_t)2;
        }
    }
    return term;
}
