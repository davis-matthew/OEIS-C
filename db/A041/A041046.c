/*
 * A041046  Numerators of continued fraction convergents to sqrt(29).
 * https://oeis.org/A041046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 21, u64 n <= 44, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041046: Numerators of convergents to √29
   Recurrence:  p_{-2} = 0,  p_{-1} = 1
   p_n = a_n * p_{n-1} + p_{n-2}
   with a_0 = 5 and the periodic part a_{k ≥ 1} = {2,1,1,2,10} repeating every 5 terms.
   a(n) = p_n
*/
seq_t a(idx_t n)
{
    seq_t p_im2 = (seq_t)0;   /* p_{-2} */
    seq_t p_im1 = (seq_t)1;   /* p_{-1} */
    static const seq_t period[5] = {
        (seq_t)2, (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)10
    };
    idx_t j = 0; /* index into period for terms a_1... */

    for (idx_t i = 0; i <= n; ++i) {
        seq_t a_term;
        if (i == 0)
            a_term = (seq_t)5;          /* a_0 */
        else {
            a_term = period[j];
            ++j;
            if (j == 5) j = 0;           /* cycle period */
        }
        seq_t p = a_term * p_im1 + p_im2;
        p_im2 = p_im1;
        p_im1 = p;
    }
    return p_im1;   /* equals p_n = a(n) */
}
