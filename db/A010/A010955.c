/*
 * A010955  Binomial coefficient C(39,n).
 * https://oeis.org/A010955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binomial(39,n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If n is outside [0,39] the binomial coefficient is defined as 0. */
    if (n > 39ULL) {
        return 0;
    }

    /* Use symmetry C(39,n) = C(39,39-n) to keep the loop short. */
    idx_t k = (n <= 39 - n) ? n : 39 - n;

    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* Multiply by (39 - i + 1) then divide by i, all in seq_t. */
        result = result * (seq_t)(39 - i + 1) / (seq_t)i;
    }
    return result;
}
