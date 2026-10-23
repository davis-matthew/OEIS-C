/*
 * A040823  Continued fraction for sqrt(853).
 * https://oeis.org/A040823
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = the nth term of the continued fraction expansion of
 *            sqrt(853) (a(0) = floor(sqrt(853)), the remaining terms repeat
 *            with period 23)
 * Range:     u8 none, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static const idx_t D = (idx_t)853;
    static seq_t a0 = 0;
    static seq_t period[32];
    static idx_t period_len = 0;
    static bool init = false;

    if (!init) {
        /* ---- compute floor(sqrt(D)) via binary search ---- */
        seq_t left  = 0;
        seq_t right = 1;
        /* increase right until right*right > D */
        while (right * right <= (seq_t)D) right <<= 1;
        /* binary search in (left, right] */
        while (right - left > 1) {
            seq_t mid = (left + right) / 2;
            if (mid * mid <= (seq_t)D) left = mid;
            else right = mid;
        }
        a0 = left;
        if (a0 * a0 > (seq_t)D) --a0;          /* safety for rounding errors */

        /* ---- compute the periodic part of the continued fraction ---- */
        seq_t m = 0;
        seq_t d = 1;
        seq_t a = a0;
        size_t idx = 0;
        do {
            m = d * a - m;
            seq_t temp = (seq_t)D - m * m;
            d = temp / d;
            a = (a0 + m) / d;
            period[idx++] = a;
        } while (a != (a0 * 2));              /* last term is 2*a0 */
        period_len = (idx > 0 ? idx : 1);
        init = true;
    }

    if (n == 0) return a0;
    idx_t k = (n - 1) % period_len;           /* 0 <= k < period_len */
    return period[(size_t)k];
}
