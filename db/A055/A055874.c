/*
 * A055874  a(n) = largest m such that 1, 2, ..., m divide n.
 * https://oeis.org/A055874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = largest integer m such that lcm(1,2,…,m) divides n
 * Range:     u8 n <= 10080, u16 n <= 10080, u32 n <= 10080, u64 n <= 10080,
 *            u128 n <= 10080, big any n
 * Validated: standard, pass, 116 terms through n = 9943 (2026-11-01)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd_u64.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    uint64_t curL = 1;      /* L(1) */
    seq_t    best   = 1;    /* a(1) is always 1 */

    /* iterate m = 2,3,… while L(m) ≤ n */
    for (idx_t m = 2; m <= n; ++m) {
        uint64_t g   = gcd_u64(curL, (uint64_t)m);
        /* compute lcm(curL , m) safely */
        unsigned __int128 prod = (unsigned __int128)(curL / g) * (uint64_t)m;
        if (prod > n) break;          /* further m will only make L larger */
        curL = (uint64_t)prod;         /* L(m) fits into 64 bits here */
        if (n % curL == 0)
            best = (seq_t)m;           /* update the maximum m found */
    }

    return best;
}
