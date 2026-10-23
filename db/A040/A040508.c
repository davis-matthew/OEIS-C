/*
 * A040508  Continued fraction for sqrt(532).
 * https://oeis.org/A040508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(n) follows the continued fraction expansion of sqrt(532).
 *            a(0)=23, a(1..)=period [15,2,1,4,2,4,1,2,15,46] repeating
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t period[10];
    static idx_t per_len;

    if (!initialized) {
        const long long D = 532;          /* square‑free part */
        long long m = 0;
        long long d = 1;
        const long long a0 = 23;         /* floor(sqrt(532)) */
        long long a = a0;
        per_len = 0;
        do {
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0 + m) / d;
            period[per_len++] = (seq_t)a;
        } while (a != 2 * a0);           /* terminate when a == 2*a0 */
        initialized = true;
    }

    if (n == 0) return (seq_t)23;
    idx_t idx = n - 1;
    return period[idx % per_len];
}
