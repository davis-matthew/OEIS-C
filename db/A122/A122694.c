/*
 * A122694  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2+(x+761)^2 = y^2.
 * https://oeis.org/A122694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3) - a(n-6) + 1522 for n > 6 a(1) = 0, a(2) = 583,
 *            a(3) = 820, a(4) = 2283, a(5) = 5440, a(6) = 6783
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 28, u64 n <= 66, u128 n <= 141,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial 6 values (index 1..6). */
    static const seq_t init[7] = {0, 0, 583, 820, 2283, 5440, 6783};

    if (n <= 6)         /* direct hit for the first 6 terms */
        return init[n];

    /* circular buffer holding the last 6 computed values.
       buf[i] corresponds to a(i+1) in 1‑based indexing. */
    seq_t buf[6];
    buf[0] = init[1];
    buf[1] = init[2];
    buf[2] = init[3];
    buf[3] = init[4];
    buf[4] = init[5];
    buf[5] = init[6];

    seq_t cur;
    for (idx_t k = 7; k <= n; ++k) {
        /* indices for a(k-3) and a(k-6) in the circular buffer */
        idx_t i_k_minus_3 = (k - 4) % 6;  /* because buf[0] = a(1) */
        idx_t i_k_minus_6 = (k - 7) % 6;  /* k >= 7, so k-7 >= 0 */

        cur = 6 * buf[i_k_minus_3] - buf[i_k_minus_6] + (seq_t)1522;
        buf[(k - 1) % 6] = cur;          /* store a(k) */
    }
    return buf[(n - 1) % 6];
}
