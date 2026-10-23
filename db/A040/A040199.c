/*
 * A040199  Continued fraction for sqrt(214).
 * https://oeis.org/A040199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Continued fraction terms of sqrt(214)
   The period length is 26. a(0)=14, and for n≥1
   a(n) = periodic_term[(n-1) % 26] where periodic_term[]
   contains the 26 terms of the continued fraction after a0.
*/
seq_t a(idx_t n)
{
    /* Periodic part of (a1,...,a26). */
    static seq_t period[26];
    static idx_t period_len = 0;

    if (period_len == 0) {
        const seq_t D  = (seq_t)214;
        const seq_t a0 = (seq_t)14;

        seq_t m = (seq_t)0;
        seq_t d = (seq_t)1;
        seq_t a = a0;
        idx_t  i = 0;

        while (1) {
            /* iterate to next convergent term */
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0 + m) / d;   /* floor division */
            period[i++] = a;
            if (a == a0 * (seq_t)2)   /* a == 2*a0 marks end of period */
                break;
        }
        period_len = i;   /* should be 26 */
    }

    if (n == (idx_t)0)
        return (seq_t)14;

    return period[(n - (idx_t)1) % period_len];
}
