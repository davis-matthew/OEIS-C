/*
 * A090769  a(n) = 7^(n^2+2n+1)*Product_{j=1..n} (49^j-1).
 * https://oeis.org/A090769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7^((n+1)^2) * Π_{j=1..n} (49^j - 1)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 7, a(n) = a(n-1) * 7^(2n+1) * (7^(2n) - 1) */

seq_t a(idx_t n)
{
    /* Base term for n = 0 */
    seq_t result = (seq_t)7;
    if (n == 0) {
        return result;
    }

    /* pow2 holds 7^(2*i) for the current i. Initialise for i = 1. */
    seq_t pow2 = (seq_t)49;   /* 7^2 */

    for (idx_t i = 1; i <= n; ++i) {
        /* (7^(2i) - 1) */
        seq_t factor = pow2 - (seq_t)1;

        /* 7^(2i+1) = 7^(2i) * 7 */
        seq_t pow2_i1 = pow2 * (seq_t)7;

        /* Update a(i) from a(i-1) */
        result = result * pow2_i1;   /* multiply by 7^(2i+1) */
        result = result * factor;    /* multiply by (7^(2i) - 1) */

        /* Prepare pow2 for the next iteration: 7^(2(i+1)) = 7^(2i) * 49 */
        pow2 = pow2 * (seq_t)49;
    }

    return result;
}
