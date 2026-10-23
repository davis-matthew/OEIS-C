/*
 * A041242  Numerators of continued fraction convergents to sqrt(133).
 * https://oeis.org/A041242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 19, u64 n <= 44, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued‑fraction convergents to sqrt(133).
 * Recurrence:
 *   p[-2] = 0,  p[-1] = 1;
 *   p[n]   = a[n] * p[n-1] + p[n-2],
 * where a[0] = 11 and for n≥1, a[n] is the (n‑1)-st element of the
 * repeating block [1,1,7,5,1,1,1,2,1,1,1,5,7,1,1,22].
 */
seq_t a(idx_t n)
{
    static const idx_t period[16] = {
        1, 1, 7, 5, 1, 1, 1, 2,
        1, 1, 1, 5, 7, 1, 1, 22
    };

    seq_t p_minus2 = 0;   /* p_{-2} */
    seq_t p_minus1 = 1;   /* p_{-1} */
    seq_t p;              /* current numerator */

    for (idx_t i = 0; i <= n; ++i) {
        seq_t a_i;
        if (i == 0) {
            a_i = (seq_t)11;
        } else {
            a_i = (seq_t)period[(i - 1) % 16];
        }

        p = a_i * p_minus1 + p_minus2;

        if (i == n) {
            return p;
        }

        p_minus2 = p_minus1;
        p_minus1 = p;
    }

    /* unreachable, but keeps compiler happy */
    return 0;
}
