/*
 * A027378  Expansion of (1+x^2-x^3)/(1-x)^4.
 * https://oeis.org/A027378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+4,3) - 3*(n+1)
 * Range:     u8 n <= 8, u16 n <= 70, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute binomial (n+4 choose 3) safely, avoiding overflow for small seq_t */
    seq_t a_val = (seq_t)n + 4;
    seq_t b_val = (seq_t)n + 3;
    seq_t c_val = (seq_t)n + 2;

    /* divide by 2 */
    if ((a_val & 1) == 0) {
        a_val >>= 1;
    } else if ((b_val & 1) == 0) {
        b_val >>= 1;
    } else {
        c_val >>= 1;
    }

    /* divide by 3 */
    if ((a_val % 3) == 0) {
        a_val /= 3;
    } else if ((b_val % 3) == 0) {
        b_val /= 3;
    } else {
        c_val /= 3;
    }

    /* product gives the binomial coefficient */
    seq_t binom = a_val * b_val * c_val;

    /* subtract the linear part */
    seq_t result = binom - 3 * ((seq_t)n + 1);

    return result;
}
