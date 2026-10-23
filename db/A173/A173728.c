/*
 * A173728  Number of reduced 3 X 3 semimagic squares with magic sum n.
 * https://oeis.org/A173728
 *
 * Offset:    12    (a(12) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n^3/2 - 36*n^2 + 264*n - 396 * + (n mod
 *            2)*(n^2-21*n+55)*9/2 * + ((2*n^2+1) mod 3)*24*(n-9) * +
 *            ((n^3+n^2+n+2) mod 4)*9*n * - 252*[(n mod 4)=0] - 144*[(n mod
 *            5)=0] * - 144*[(n mod 6)=0] - 72*[(n mod 7)=0] - 72*[(n mod
 *            8)=0] * Offset: 12 (a(12)=72)
 * Range:     u8 n <= 13, u16 n <= 42, u32 n <= 1428, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 12) return (seq_t)0;      /* values below offset undefined */

    /* All intermediate arithmetic fits in signed 64‑bit for n ≤ 10000 */
    int64_t nn = (int64_t)n;

    /* 3*n^3/2 + ((n mod 2)*(n^2-21*n+55)*9)/2  */
    int64_t N = 3LL * nn * nn * nn;     /* 3*n^3 */
    if (nn & 1LL)                      /* n odd → add the extra term */
    {
        int64_t t = nn * nn - 21LL * nn + 55LL;
        N += 9LL * t;                  /* 9*(n^2-21*n+55) added to numerator */
    }
    int64_t val = N / 2;                /* numerator already even */

    /* -36*n^2 + 264*n - 396 */
    val += -36LL * nn * nn + 264LL * nn - 396LL;

    /* ((2*n^2+1) mod 3)*24*(n-9) */
    int64_t mod3 = (2LL * nn * nn + 1LL) % 3LL;
    if (mod3 != 0LL)
        val += mod3 * 24LL * (nn - 9LL);

    /* ((n^3+n^2+n+2) mod 4)*9*n */
    int64_t sum = nn * nn * nn + nn * nn + nn + 2LL;
    int64_t mod4 = sum % 4LL;
    if (mod4 != 0LL)
        val += mod4 * 9LL * nn;

    /* Periodic subtractions */
    if (nn % 4LL == 0LL) val -= 252LL;
    if (nn % 5LL == 0LL) val -= 144LL;
    if (nn % 6LL == 0LL) val -= 144LL;
    if (nn % 7LL == 0LL) val -= 72LL;
    if (nn % 8LL == 0LL) val -= 72LL;

    return (seq_t)val;
}
