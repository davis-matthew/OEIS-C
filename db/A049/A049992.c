/*
 * A049992  a(n) is the number of arithmetic progressions of 3 or more
 *          positive integers, nondecreasing with sum n.
 * https://oeis.org/A049992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of arithmetic progressions (a, a+d, …, a+(k-1)d) of
 *            length k ≥ 3 with positive integers, nondecreasing (d ≥ 0), and
 *            sum = n. Equivalent counting: for each k ≥ 3, count d ≥ 0 such
 *            that n – d*k*(k-1)/2 > 0 and (n – d*k*(k-1)/2) is divisible by k
 * Range:     u8 n <= 43, u16 n <= 3057, u32 n <= 9949, u64 n <= 9949,
 *            u128 n <= 9949, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;                    /* accumulator for the count */

    for (idx_t k = 3; k <= n; ++k) {
        /* T_k = k*(k-1)/2  (total increment contributed by d) */
        seq_t T = ((seq_t)k) * ((seq_t)k - 1) / 2;

        /* n must be at least k in order that a ≥ 1 */
        if (n < k) {
            break;
        }

        /* Maximum d such that the average a stays ≥ 1:
           n – d*T ≥ k   →   d ≤ (n – k)/T
        */
        idx_t dmax = (idx_t)((((seq_t)n) - (seq_t)k) / T);

        for (idx_t d = 0; d <= dmax; ++d) {
            /* Remaining sum after subtracting the contribution of d */
            seq_t rem = ((seq_t)n) - ((seq_t)d) * T;

            /* Check divisibility by k to obtain an integer a ≥ 1 */
            if ((rem % k) == 0) {
                ++result;
            }
        }
    }

    return result;
}
