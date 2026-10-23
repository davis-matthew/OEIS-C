/*
 * A045113  Numbers whose base-4 representation contains no 1's and exactly
 *          four 3's.
 * https://oeis.org/A045113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS A045113: numbers whose base‑4
   representation contains no digit 1 and exactly four 3's.  The terms
   are listed in increasing numerical order. */
#include "../utils/pow2.h"

static seq_t choose(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * ((seq_t)(n - k + i));
        res = res / ((seq_t)i);
    }
    return res;
}

static seq_t count_len(idx_t len)
{
    if (len < 4) return 0;
    seq_t term1 = choose(len, 4) * pow2(len - 4);
    seq_t term2 = 0;
    if (len >= 5) {
        term2 = choose(len - 1, 4) * pow2(len - 5);
    }
    return term1 - term2;
}

seq_t a(idx_t n)
{
    seq_t rem = (seq_t)n;
    idx_t len = 4;
    while (1) {
        seq_t cnt = count_len(len);
        if (rem <= cnt) break;
        rem -= cnt;
        ++len;
    }
    seq_t value = 0;
    idx_t remaining3 = 4;
    for (idx_t pos = 0; pos < len; ++pos) {
        seq_t candidates[3];
        int nc = 0;
        if (pos == 0) {
            candidates[nc++] = 2;
            candidates[nc++] = 3;
        } else {
            candidates[nc++] = 0;
            candidates[nc++] = 2;
            candidates[nc++] = 3;
        }
        seq_t chosen = 0;
        for (int i = 0; i < nc; ++i) {
            seq_t d = candidates[i];
            if (d == 3 && remaining3 == 0) continue;
            idx_t remPos = len - pos - 1;
            idx_t rem3After = remaining3 - (d == 3 ? 1 : 0);
            seq_t cnt = 0;
            if (rem3After <= remPos) {
                seq_t bin = choose(remPos, rem3After);
                seq_t pw = pow2(remPos - rem3After);
                cnt = bin * pw;
            }
            if (rem <= cnt) {
                chosen = d;
                remaining3 = rem3After;
                value = value * 4 + chosen;
                break;
            } else {
                rem -= cnt;
            }
        }
    }
    return value;
}
