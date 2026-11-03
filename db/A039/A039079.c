/*
 * A039079  Numbers whose base-9 representation has the same number of 0's and
 *          3's.
 * https://oeis.org/A039079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th positive integer whose base-9 representation has
 *            an equal number of 0's and 3's
 * Range:     u8 n <= 167, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9941 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* test whether a uint64_t value has the same number of 0 and 3 digits
   in its base‑9 representation */
static bool same_zero_three_uint64(uint64_t x)
{
    uint8_t cnt0 = 0, cnt3 = 0;
    while (x > 0) {
        uint8_t d = (uint8_t)(x % 9);
        if (d == 0)  cnt0++;
        else if (d == 3) cnt3++;
        x /= 9;
    }
    return cnt0 == cnt3;
}

/* OEIS sequence A039079 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, just for safety */
    uint64_t found = 0;
    uint64_t cand   = 1;           /* start from 1 */
    while (1) {
        if (same_zero_three_uint64(cand)) {
            if (++found == n)
                return (seq_t)cand;
        }
        cand++;                      /* next candidate */
    }
}
