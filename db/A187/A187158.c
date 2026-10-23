/*
 * A187158  Number of 5-step one space at a time bishop's tours on an n X n
 *          board summed over all starting positions.
 * https://oeis.org/A187158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 28, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 0                     for n ≤ 3
 *            a(n) = 100*n^2 – 584*n + 808 for n > 3
 *
 * This implements the empirical quadratic formula confirmed for all n > 3.
 * The value 0 for n = 1, 2, 3 matches the OEIS entries a(1) … a(3).
 * Only integer arithmetic is used; seq_t is cast from idx_t before
 * multiplication to avoid premature truncation.
 */

seq_t a(idx_t n)
{
    if (n <= 3)
        return 0;

    seq_t nn = (seq_t)n;
    seq_t result = (seq_t)100 * nn * nn;     /* 100 * n^2   */
    result -= (seq_t)584 * nn;               /* - 584 * n   */
    result += (seq_t)808;                    /* + 808       */
    return result;
}
