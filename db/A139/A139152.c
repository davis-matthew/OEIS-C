/*
 * A139152  a(n) = (n!+5)/5.
 * https://oeis.org/A139152
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n! + 5) / 5
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 5.  For smaller n return 0. */
    if (n < 5) return 0;

    seq_t f = 1;          /* running product of the factorial, with one factor of 5 removed */
    idx_t div_pending = 1;/* we must divide by 5 exactly once */

    for (idx_t i = 2; i <= n; ++i) {
        f = f * (seq_t)i;          /* multiply by the next factor */
        if (div_pending && (f % 5 == 0)) {
            f /= 5;                /* remove one factor of 5 */
            div_pending = 0;
        }
    }

    /* f now holds n!/5 for n >= 5.  The final formula adds 1. */
    return f + 1;
}
