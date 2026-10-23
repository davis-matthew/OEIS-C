/*
 * A040772  Continued fraction for sqrt(801).
 * https://oeis.org/A040772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 28, a(n) = continued fraction term of √801 for n ≥ 1
 * Range:     u8 none, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* squared root expansion for D = 801 */
    static int init = 0;
    static seq_t period[64];
    static idx_t period_len = 0;

    if (!init) {
        const seq_t N     = (seq_t)801;
        const seq_t a0    = (seq_t)28;
        const seq_t two_a0= a0 + a0;
        seq_t m = (seq_t)0;
        seq_t d = (seq_t)1;
        seq_t a = a0;            /* a holds the previous term */

        /* generate period terms until the final 2*a0 appears */
        do {
            /* advance to next term */
            m = d * a - m;
            d = (N - m * m) / d;
            a = (a0 + m) / d;
            period[period_len++] = a;
        } while (a != two_a0);

        init = 1;
    }

    if (n == (idx_t)0)          /* the integer part */
        return (seq_t)28;

    /* shift index because period[0] is a1 */
    idx_t idx = (n - (idx_t)1) % period_len;
    return period[idx];
}
