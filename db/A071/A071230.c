/*
 * A071230  a(n) = n*(6*n^2 - 7*n + 3)/2.
 * https://oeis.org/A071230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 28, u32 n <= 912, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n*(6*n^2 - 7*n + 3)/2
 *
 * Computes the nth term of OEIS A071230.
 * The function works for any build where seq_t and idx_t are defined.
 * It uses only arithmetic operations allowed by the requirements.
 */
seq_t a(idx_t n)
{
    /* Convert index to seq_t to avoid overflow in intermediate calculations. */
    seq_t nn = (seq_t)n;

    /* Compute 6*n^2 - 7*n + 3 */
    seq_t term = 6 * nn * nn;   /* 6*n^2   */
    term -= 7 * nn;             /* - 7*n   */
    term += 3;                  /* + 3     */

    /* Multiply by n and divide by 2 */
    return nn * term / 2;
}
