/*
 * A045028  Numbers whose base-4 representation contains exactly three 0's and
 *          no 1's.
 * https://oeis.org/A045028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base‑4 representation contains exactly three 0's
 *            and no 1's
 * Range:     u8 n <= 2, u16 n <= 1500, u32 n <= 1500, u64 n <= 1500,
 *            u128 n <= 1500, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"

/* Count sequences of length L (L≥4) with exactly 3 zeros, no 1's,
   leading digit 2 or 3. */
static inline seq_t count_len(idx_t L)
{
    if (L < 4) return 0;
    idx_t rem = L - 1;                       /* positions after the leading digit */
    /* C(rem,3) */
    seq_t comb = (seq_t)rem * (rem - 1) * (rem - 2) / 6;
    seq_t powv = pow2(rem - 3);              /* 2^(rem‑3) = 2^(L‑4) */
    return 2 * comb * powv;                  /* factor 2 for leading digit 2 or 3 */
}

/* Count suffixes of length rem that use exactly zeros zeros
   (digits 0) and the rest 2 or 3. */
static inline seq_t count_suffix(idx_t rem, idx_t zeros)
{
    if (zeros > rem) return 0;
    seq_t comb;
    if (zeros == 0)          comb = 1;
    else if (zeros == 1)     comb = (seq_t)rem;
    else if (zeros == 2)     comb = (seq_t)rem * (rem - 1) / 2;
    else if (zeros == 3)     comb = (seq_t)rem * (rem - 1) * (rem - 2) / 6;
    else                     comb = 0;

    seq_t powv = pow2(rem - zeros);          /* 2^(remaining non‑zero positions) */
    return comb * powv;
}

seq_t a(idx_t n)
{
    /* Find the length L of the nth number. */
    seq_t cumulative = 0;
    idx_t L;
    for (L = 4; ; ++L) {
        seq_t cnt = count_len(L);
        if (cumulative + cnt >= (seq_t)n)
            break;
        cumulative += cnt;
    }

    /* Index within the block of length L, zero‑based. */
    seq_t rank = (seq_t)n - cumulative - 1;

    /* Count for a fixed leading digit (2 or 3). */
    seq_t blockcnt = count_suffix(L - 1, 3);

    seq_t value;
    if (rank < blockcnt) {
        value = 2;
    } else {
        rank -= blockcnt;
        value = 3;
    }

    idx_t rem = L - 1;          /* positions after the leading digit */
    idx_t zeros_left = 3;

    for (idx_t pos = 0; pos < rem; ++pos) {
        if (zeros_left > 0) {
            seq_t cnt0 = count_suffix(rem - pos - 1, zeros_left - 1);
            if (rank < cnt0) {
                value = (value << 2) | 0;
                zeros_left--;
                continue;
            }
            rank -= cnt0;
        }

        seq_t cnt2 = count_suffix(rem - pos - 1, zeros_left);
        if (rank < cnt2) {
            value = (value << 2) | 2;
            continue;
        }
        rank -= cnt2;

        value = (value << 2) | 3;
    }

    return value;
}
