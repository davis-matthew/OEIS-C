/*
 * A041562  Numerators of continued fraction convergents to sqrt(299).
 * https://oeis.org/A041562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(299).
   The continued‑fraction coefficients of sqrt(299) are:
   a(0)=17, a(1)=3, a(2)=2, a(3)=3, a(4)=34, and thereafter
   [3,2,3,34] repeats. The numerators p_n satisfy
   p_{-2}=0, p_{-1}=1, p_n = a_n * p_{n-1} + p_{n-2} for n>=0.
*/
seq_t a(idx_t n)
{
    /* period of the continued fraction after the first term */
    static const int period[4] = {3, 2, 3, 34};

    seq_t prev_prev = 0; /* p_{k-2}   */
    seq_t prev      = 1; /* p_{k-1}   */
    seq_t cur       = 0; /* p_k       */

    for (idx_t i = 0; i <= n; ++i) {
        seq_t coeff;
        if (i == 0)
            coeff = (seq_t)17; /* a_0 = 17 */
        else
            coeff = (seq_t)period[(i - 1) % 4]; /* periodic part */

        cur = coeff * prev + prev_prev;
        prev_prev = prev;
        prev = cur;
    }
    return cur;
}
