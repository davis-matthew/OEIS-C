/*
 * A134358  Coefficient of y^(n-3) in expansion of (y+n!)^n.
 * https://oeis.org/A134358
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n!)^3 * n * (n-1) * (n-2) / 6 (for n >= 3)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return 0;            /* offset 3, values below are unspecified */

    seq_t res = (seq_t)1;
    int need2 = 1;   /* one factor 2 to cancel (for division by 6 = 2*3) */
    int need3 = 1;   /* one factor 3 to cancel */

    /* multiply (n!)^3 : each i from 1..n appears three times */
    for (idx_t i = 1; i <= n; ++i) {
        for (int k = 0; k < 3; ++k) {
            seq_t f = i;

            /* cancel factor 2 if still needed */
            if (need2 && ((f & 1) == 0)) {
                f >>= 1;
                --need2;
            }

            /* cancel factor 3 if still needed */
            if (need3 && (f % (seq_t)3 == 0)) {
                f /= (seq_t)3;
                --need3;
            }

            res *= f;
        }
    }

    /* multiply the extra factors n, n-1, n-2 */
    seq_t f = n;
    if (need2 && ((f & 1) == 0)) { f >>= 1; --need2; }
    if (need3 && (f % (seq_t)3 == 0)) { f /= (seq_t)3; --need3; }
    res *= f;

    f = n - 1;
    if (need2 && ((f & 1) == 0)) { f >>= 1; --need2; }
    if (need3 && (f % (seq_t)3 == 0)) { f /= (seq_t)3; --need3; }
    res *= f;

    f = n - 2;
    if (need2 && ((f & 1) == 0)) { f >>= 1; --need2; }
    if (need3 && (f % (seq_t)3 == 0)) { f /= (seq_t)3; --need3; }
    res *= f;

    return res;
}
