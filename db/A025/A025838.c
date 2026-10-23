/*
 * A025838  Expansion of 1/((1-x^3)*(1-x^5)*(1-x^10)).
 * https://oeis.org/A025838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2 + 42*n + 348 - 6*(n+4)*((n mod 5) + ((n+2) mod
 *            5)))/300)
 * Range:     u8 none, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute each part in seq_t to avoid overflow */
    seq_t n_sq    = (seq_t)n * (seq_t)n;           /* n^2 */
    seq_t term    = n_sq + 42 * (seq_t)n + 348;    /* n^2 + 42n + 348 */

    idx_t r1 = n % 5;                /* n mod 5 */
    idx_t r2 = (n + 2) % 5;          /* (n+2) mod 5 */
    seq_t r   = (seq_t)(r1 + r2);    /* sum of the two remainders */

    seq_t mult = 6 * ((seq_t)(n + 4)) * r;  /* 6*(n+4)*((n mod5)+(n+2 mod5)) */

    seq_t num = term - mult;         /* numerator, always nonnegative for n>=0 */

    return num / 300;                /* floor division */
}
