/*
 * A045064  Numbers whose base-4 representation contains no 0's and exactly
 *          three 3's.
 * https://oeis.org/A045064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   binom_rem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_rem.h"
/* Computation: nth positive integer whose base‑4 representation
   contains no 0's and exactly three 3's (in increasing order). */

/* returns 2^e as a seq_t */
static seq_t pow2u(idx_t e)
{
    seq_t v = 1;
    for (idx_t i = 0; i < e; ++i)
        v <<= 1;
    return v;
}

/* number of valid numbers of length L (digits in base‑4) */
static seq_t count_len(idx_t L)
{
    if (L < 3) return 0;
    return binom_rem(L, 3) * pow2u(L - 3);
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for indices < 1 */

    seq_t target = (seq_t)n;
    seq_t cum    = 0;
    idx_t L      = 3;

    /* locate the block containing the n-th number */
    while (1) {
        seq_t cnt = count_len(L);
        if (cum + cnt >= target) break;
        cum += cnt;
        ++L;
    }

    seq_t idx_in_block = target - cum;  /* 1‑based position within this block */
    idx_t rem_pos = L;
    idx_t rem_3s  = 3;
    seq_t value   = 0;

    for (idx_t pos = 0; pos < L; ++pos) {
        for (int d = 1; d <= 3; ++d) {
            if (d == 3 && rem_3s == 0) continue;

            idx_t new_rem_3s = rem_3s - (d == 3 ? 1 : 0);
            idx_t new_rem_pos = rem_pos - 1;

            if (new_rem_pos < new_rem_3s) continue;

            seq_t ways = binom_rem(new_rem_pos, new_rem_3s)
                         * pow2u(new_rem_pos - new_rem_3s);

            if (idx_in_block > ways) {
                idx_in_block -= ways;
                continue;
            }

            /* accept this digit */
            value = value * 4 + (seq_t)d;
            rem_3s = new_rem_3s;
            rem_pos = new_rem_pos;
            break;
        }
    }

    return value;
}
