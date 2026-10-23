/*
 * A015166  Inverse of 1157th cyclotomic polynomial.
 * https://oeis.org/A015166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if 0 ≤ n mod 1157 ≤ 12, a(n) = -1 if 89 ≤ n mod 1157 ≤
 *            101, otherwise a(n) = 0. The sequence is periodic with period
 *            1157
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Period length */
    const idx_t PERIOD = 1157ULL;
    /* Boundaries of non‑zero entries */
    const idx_t P = 13ULL;  /* number of leading 1’s */
    const idx_t Q = 89ULL;  /* start of the block of -1’s */

    /* Compute residue modulo 1157 */
    idx_t r = n % PERIOD;

    /* Leading block of 1’s */
    if (r < P)
        return (seq_t)1;

    /* Block of -1’s */
    if (r >= Q && r < Q + P)
        return (seq_t)-1;

    /* All other indices */
    return (seq_t)0;
}
