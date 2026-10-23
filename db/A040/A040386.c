/*
 * A040386  Continued fraction for sqrt(407).
 * https://oeis.org/A040386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = ⌊√407⌋ = 20; for n ≥ 1, a(n) is the nth partial quotient
 *            of the continued‑fraction expansion of √407. The period is found
 *            by the classic quadratic‑surd algorithm. The sequence repeats
 *            with the period [5, 1, 2, 1, 5, 40]
 * Range:     u8 none, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    static bool init = false;
    static seq_t a0;              /* integer part of √407 */
    static seq_t period[256];     /* period terms (a1 … a_k) */
    static idx_t period_len = 0;  /* length of the period */

    if (!init) {
        const seq_t D = (seq_t)407;
        a0 = floor_sqrt(D);          /* a0 = 20 for √407 */

        seq_t m = 0;
        seq_t d = 1;
        seq_t a = a0;

        idx_t i = 0;
        while (1) {
            /* m_{k+1} = d_k * a_k - m_k */
            m = d * a - m;
            /* d_{k+1} = (D - m_{k+1}²) / d_k */
            d = (D - m * m) / d;
            /* a_{k+1} = ⌊(a0 + m_{k+1}) / d_{k+1}⌋ */
            a = (a0 + m) / d;
            period[i++] = a;
            if (a == 2 * a0)          /* period terminates when a = 2*a0 */
                break;
        }
        period_len = i;
        init = true;
    }

    if (n == 0)
        return a0;
    /* n >= 1: use the periodic part */
    idx_t pos = (n - 1) % period_len;
    return period[pos];
}
