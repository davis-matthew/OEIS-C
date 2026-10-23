/*
 * A045042  Numbers whose base-4 representation contains exactly one 0 and no
 *          2's.
 * https://oeis.org/A045042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 1539, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A045042: numbers whose base‑4 representation contains exactly one 0
   and no 2's (offset 1).  The function generates the nth term combinatorially
   without scanning all integers. */
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(1) = 0 */
    if (n == 1) return (seq_t)0;

    /* Remaining index after the leading zero */
    idx_t m = n - 1;

    /* Find the length L of the base‑4 representation */
    idx_t L = 2;
    while (1) {
        idx_t cnt_len;
        if (L-1 > 63) {
            cnt_len = UINT64_MAX;
        } else {
            cnt_len = (L-1) * ((idx_t)1 << (L-1));
        }
        if (m <= cnt_len) break;
        m -= cnt_len;
        ++L;
    }

    /* Build the number digit by digit */
    seq_t value = 0;
    bool zeroUsed = false;

    for (idx_t pos = 0; pos < L; ++pos) {
        idx_t rem = L - pos - 1;

        /* Candidate digits in increasing order */
        int cand[3], ncand;
        if (pos == 0) {          /* most significant digit cannot be 0 */
            cand[0] = 1; cand[1] = 3; ncand = 2;
        } else {
            cand[0] = 0; cand[1] = 1; cand[2] = 3; ncand = 3;
        }

        for (int i = 0; i < ncand; ++i) {
            int d = cand[i];
            if (d == 0 && zeroUsed) continue;  /* already used a zero */
            if (d == 0 && pos == 0) continue;  /* leading zero not allowed */

            bool zeroNow = zeroUsed || (d == 0);
            idx_t compIdx;

            if (rem == 0) {
                compIdx = zeroNow ? 1 : 0;
            } else {
                if (zeroNow) {  /* no more zeros allowed */
                    if (rem > 63) compIdx = UINT64_MAX;
                    else          compIdx = ((idx_t)1 << rem);
                } else {        /* one zero must appear in the remaining digits */
                    if (rem - 1 > 63) compIdx = UINT64_MAX;
                    else              compIdx = (idx_t)rem * (((idx_t)1) << (rem - 1));
                }
            }

            if (m <= compIdx) {
                value = value * 4 + (seq_t)d;
                zeroUsed = zeroNow;
                break;
            } else {
                m -= compIdx;
            }
        }
    }

    return value;
}
