/*
 * A210281  a(n) = (8n)!/8^n.
 * https://oeis.org/A210281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8n)! / 8^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*                                            (computed by multiplying each block of 8 consecutive integers and dividing by 8 per block) */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;          /* a(0) = 1 */
    if (n == 0)
        return result;

    for (idx_t k = 1; k <= n; ++k) {
        /* multiply by the 8 numbers (8k-7),(8k-6),...,(8k) */
        for (idx_t j = 0; j < 8; ++j) {
            result *= (seq_t)(8 * k - j);
        }
        /* the product contains a factor 8, so division is exact */
        result /= (seq_t)8;
    }
    return result;
}
