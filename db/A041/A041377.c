/*
 * A041377  Denominators of continued fraction convergents to sqrt(203).
 * https://oeis.org/A041377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 114*a(n-2) - a(n-4) with initial values a(0)=1, a(1)=4,
 *            a(2)=113, a(3)=456
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 4;
    if (n == 2) return 113;
    if (n == 3) return 456;

    seq_t prev4 = 1;   /* a(n-4) */
    seq_t prev3 = 4;   /* a(n-3) */
    seq_t prev2 = 113; /* a(n-2) */
    seq_t prev1 = 456; /* a(n-1) */
    seq_t cur = 0;

    for (idx_t i = 4; i <= n; ++i) {
        cur = 114 * prev2 - prev4;
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
