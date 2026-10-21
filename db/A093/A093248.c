/*
 * A093248  a(n) is the largest number such that all of a(n)'s length-n
 *          substrings are distinct and divisible by 48.
 * https://oeis.org/A093248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = (n‑2) digits of 9, followed by 36, then n zeros for n ≥
 *            4. Special cases: a(1)=0, a(2)=96, a(3)=6720. The returned value
 *            is the largest integer whose all length‑n substrings are
 *            distinct and divisible by 48
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t val = 0;

    if (n < 4) {
        /* special cases */
        switch (n) {
            case 1u: return (seq_t)0;
            case 2u: return (seq_t)96;
            case 3u: return (seq_t)6720;
            default: return (seq_t)0;   /* unreachable */
        }
    }

    /* leading n-2 digits of 9 */
    idx_t i;
    for (i = 0; i < n - 2; ++i) {
        val = val * (seq_t)10 + (seq_t)9;
    }
    /* append 3 and 6 */
    val = val * (seq_t)10 + (seq_t)3;
    val = val * (seq_t)10 + (seq_t)6;
    /* append n zeros: multiply by 10^n */
    val = val * pow10(n);

    return val;
}
