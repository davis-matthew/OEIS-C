/*
 * A202156  y-values in the solution to x^2 - 13*y^2 = -1.
 * https://oeis.org/A202156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = y part of (18 + 5√13)^(2n‑1), the y-coordinate of the
 *            n-th solution to x² − 13·y² = −1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* undefined for n < 1, return 0 by convention */

    /* exponent to which the fundamental unit is raised: 2·n−1 */
    idx_t exp = 2 * n - 1;

    /* start with the identity (1, 0) and the base (18, 5) */
    seq_t res_x = 1, res_y = 0;       /* (x, y) = 1 + 0√13 */
    seq_t base_x = 18, base_y = 5;    /* 18 + 5√13 */

    while (exp > 0) {
        if (exp & 1) {                 /* res ← res * base */
            seq_t new_x = res_x * base_x + (seq_t)13 * res_y * base_y;
            seq_t new_y = res_x * base_y + res_y * base_x;
            res_x = new_x;
            res_y = new_y;
        }
        /* base ← base * base */
        seq_t new_base_x = base_x * base_x + (seq_t)13 * base_y * base_y;
        seq_t new_base_y = (seq_t)2 * base_x * base_y;
        base_x = new_base_x;
        base_y = new_base_y;
        exp >>= 1;
    }
    return res_y;                      /* the required y-value */
}
