/*
 * A055268  a(n) = (11*n + 4)*C(n+3, 3)/4.
 * https://oeis.org/A055268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 4)*C(n+3,3)/4
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 250, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   = ((11*n + 4) * (n+3)*(n+2)*(n+1)) / 24,  n >= 0
 * This implementation uses only integer arithmetic and works
 * for any seq_t that supports the operators +, -, *, /, and
 * the assignment from idx_t.  All intermediate calculations
 * fit exactly because the binomial coefficient (n+3 choose 3)
 * is divisible by 6, and the final product is divisible by 4.
 */
seq_t a(idx_t n) {
    /* Convert index to seq_t for safe arithmetic */
    seq_t nn = (seq_t)n;

    /* Compute the binomial coefficient (n+3 choose 3) */
    seq_t bin = (nn + (seq_t)3) * (nn + (seq_t)2) * (nn + (seq_t)1);
    bin /= 6;                         /* divide by 6 – exact */

    /* Compute (11*n + 4) * binomial / 4 */
    seq_t part = 11 * nn + (seq_t)4;
    seq_t result = part * bin;
    result /= 4;                       /* divide by 4 – exact */

    return result;
}
