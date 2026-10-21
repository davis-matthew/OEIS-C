/*
 * A216729  a(n) = (3*n)!*((3*n+1)/6)^n/(3*n+1).
 * https://oeis.org/A216729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((3*n)!) * ((3*n + 1)^(n-1)) / 6^n (n ≥ 1, a(0)=1)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    /* number of factors 2 and 3 to cancel from the denominator 6^n = 2^n * 3^n */
    idx_t need2 = n;
    idx_t need3 = n;

    seq_t result = 1;
    idx_t three_n = 3 * n;           /* 3*n  */
    seq_t base = three_n + 1;        /* 3*n + 1 */

    /* multiply by 1..3*n, cancelling factors 2 and 3 as early as possible */
    for (idx_t i = 1; i <= three_n; ++i) {
        seq_t t = (seq_t)i;

        while (need2 > 0 && t % 2 == 0) {   /* cancel factor 2 */
            t /= 2;
            --need2;
        }
        while (need3 > 0 && t % 3 == 0) {   /* cancel factor 3 */
            t /= 3;
            --need3;
        }
        result *= t;
    }

    /* multiply by (3*n+1)^(n-1), again cancelling any remaining denominators */
    for (idx_t k = 1; k <= n - 1; ++k) {
        seq_t t = base;    /* (3*n+1) */

        while (need2 > 0 && t % 2 == 0) {
            t /= 2;
            --need2;
        }
        while (need3 > 0 && t % 3 == 0) {
            t /= 3;
            --need3;
        }
        result *= t;
    }

    return result;
}
