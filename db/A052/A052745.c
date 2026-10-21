/*
 * A052745  Expansion of e.g.f. log(-1/(-1+x))^2*x.
 * https://oeis.org/A052745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*n*Stirling1(n-1,2) with Stirling1 the signless Stirling
 *            number of the first kind (number of permutations of n-1 with 2
 *            cycles)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0), a(1) and a(2) are 0 */
    if (n < 3)
        return 0;

    /* c_prev holds c(i,2) for the current i while iterating */
    seq_t c_prev = 0;          /* c(1,2) = 0  */
    seq_t fact = 1;            /* (i-2)! for i = 2  */

    /* Compute c(i,2) for i = 2 .. n-1 */
    for (idx_t i = 2; i <= n - 1; ++i) {
        seq_t new_c;

        if (i == 2) {
            new_c = 1;                    /* c(2,2) */
        } else {
            new_c = fact + (seq_t)(i - 1) * c_prev;
        }

        c_prev = new_c;
        fact = fact * (seq_t)(i - 1);     /* fact now holds (i-1)! for next iteration */
    }

    /* a(n) = 2 * n * c(n-1,2) */
    seq_t result = (seq_t)2 * (seq_t)n * c_prev;
    return result;
}
