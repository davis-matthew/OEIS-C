/*
 * A136693  Final nonzero digit of n! in base 6.
 * https://oeis.org/A136693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = final non‑zero digit of n! in base 6
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   pow2_mod6, pow3_mod6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2_mod6.h"
#include "../utils/pow3_mod6.h"

seq_t a(idx_t n)
{
    idx_t cnt2 = 0, cnt3 = 0;
    seq_t res = (seq_t)1;

    for (idx_t i = 1; i <= n; ++i) {
        idx_t x = i;
        while ((x & 1) == 0) {          /* factor out 2 */
            x >>= 1;
            ++cnt2;
        }
        while (x % 3 == 0) {            /* factor out 3 */
            x /= 3;
            ++cnt3;
        }
        /* x is no longer divisible by 2 or 3, thus x%6 is 1 or 5 */
        res = (res * (x % 6)) % 6;
    }

    idx_t k   = (cnt2 < cnt3) ? cnt2 : cnt3;
    idx_t rem2 = cnt2 - k;   /* remaining 2‑factors */
    idx_t rem3 = cnt3 - k;   /* remaining 3‑factors */

    res = (res * pow2_mod6(rem2)) % 6;
    res = (res * pow3_mod6(rem3)) % 6;

    return res;          /* always 1..5 for n≥0 */
}
