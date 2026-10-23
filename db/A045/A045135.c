/*
 * A045135  Numbers whose base-4 representation contains no 2's and exactly
 *          two 3's.
 * https://oeis.org/A045135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 1792, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045135: numbers whose base‑4 representation contains no 2's
   and exactly two 3's.  The algorithm builds numbers by length,
   counts how many of each length exist, then generates the required
   number lexicographically within its length. */

#include <stdbool.h>
#include "../utils/pow2_int.h"

/* Number of admissible strings of length L (L >= 2) */
static seq_t count_len(idx_t L)
{
    if (L < 2)
        return 0;
    seq_t res = 0;

    /* leading digit is 3 */
    res += (seq_t)(L - 1) * pow2_int((int)L - 2);

    /* leading digit is 1 */
    if (L >= 3) {
        seq_t comb = (seq_t)(L - 1) * (L - 2) / 2;
        res += comb * pow2_int((int)L - 3);
    }
    return res;
}

/* Recursive generator in lexicographic order.  When a full‑length
   string with exactly two 3's is formed, increment counter and
   compare to targetIdx; if equal, store the value. */
static void gen_rec(idx_t pos, idx_t L, idx_t used3, seq_t val,
                    seq_t targetIdx, seq_t *counter,
                    seq_t *result, bool *found)
{
    if (*found)
        return;
    if (pos == L) {
        if (used3 == 2) {
            (*counter)++;
            if (*counter == targetIdx) {
                *result = val;
                *found = true;
            }
        }
        return;
    }
    for (int d = 0; d <= 3; ++d) {
        if (pos == 0 && d == 0)            /* no leading zero */
            continue;
        if (d == 2)                       /* digit 2 not allowed */
            continue;
        idx_t newUsed3 = used3 + (d == 3);
        if (newUsed3 > 2)
            continue;
        /* ensure enough positions left to place remaining 3's */
        if (newUsed3 + (L - pos - 1) < 2)
            continue;
        seq_t newVal = val * 4 + d;
        gen_rec(pos + 1, L, newUsed3, newVal,
                targetIdx, counter, result, found);
        if (*found)
            return;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for n < 1 */

    seq_t cumulative = 0;
    idx_t L = 2;
    /* Find the length that contains the n-th term */
    while (1) {
        seq_t cnt = count_len(L);
        if (cumulative + cnt >= (seq_t)n)
            break;
        cumulative += cnt;
        L++;
    }

    seq_t targetIdx = (seq_t)n - cumulative;  /* 1‑based within length L */
    seq_t counter = 0;
    seq_t ans = 0;
    bool found = false;

    gen_rec(0, L, 0, 0, targetIdx, &counter, &ans, &found);

    return ans;
}
