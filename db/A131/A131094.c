/*
 * A131094  Triangle where n-th row contains the smallest n positive integers
 *          (listed in order) with exactly n nonleading 0's in their binary
 *          representations.
 * https://oeis.org/A131094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 106, u32 n <= 466, u64 n <= 1954,
 *            u128 n <= 7875, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A131094:  the smallest numbers with r non‑leading zeros
   (binary), listed row‑wise.  */
#include <stdint.h>
#include <limits.h>
#include <stdbool.h>
#include "../utils/pow2.h"

/* Return C(n,k) if it does not exceed limit, otherwise return limit+1. */
static unsigned long long comb_leq(uint64_t n, uint64_t k, uint64_t limit)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;

    unsigned __int128 acc = 1;
    for (uint64_t i = 1; i <= k; ++i) {
        acc = acc * (n - k + i) / i;
        if (acc > limit) return limit + 1;
    }
    return (unsigned long long)acc;
}

seq_t a(idx_t n)
{
    /* --- locate the row r containing the n‑th term ----------------- */
    idx_t r = 1;
    idx_t cum = 0;
    while (cum + r < n) {
        cum += r;
        ++r;
    }
    idx_t offset = n - cum;          /* 1‑based position inside the row */

    /* --- find the minimal bit‑length L for this offset ------------ */
    idx_t L = r + 1;
    idx_t rem = offset;
    while (true) {
        unsigned long long cnt = comb_leq((uint64_t)(L - 1), (uint64_t)r, (uint64_t)rem);
        if (rem <= cnt)
            break;
        rem -= cnt;
        ++L;
    }

    /* If the integer would overflow a fixed‑size seq_t, return 0. */
    if (sizeof(seq_t) <= sizeof(uint64_t)) {
        idx_t max_bits = (idx_t)(sizeof(seq_t) * CHAR_BIT);
        if (L - 1 >= max_bits)
            return (seq_t)0;
    }

    /* --- construct the lower (L‑1) bits -------------------------------- */
    idx_t rem_zeros = r;          /* zeros that still need to be placed   */
    idx_t kth = rem;              /* 1‑based rank within the L‑block      */
    seq_t low = 0;

    for (int64_t pos = (int64_t)(L - 2); pos >= 0; --pos) {
        if (rem_zeros == 0) {
            /* All remaining bits must be 1. */
            low |= (seq_t)1 << pos;
            continue;
        }
        unsigned long long ways0 = comb_leq((uint64_t)pos,
                                            (uint64_t)(rem_zeros - 1),
                                            (uint64_t)kth);
        if (kth <= ways0) {
            /* Choose 0 here: consume a zero. */
            rem_zeros--;
        } else {
            /* Choose 1. */
            low |= (seq_t)1 << pos;
            kth -= ways0;
        }
    }

    seq_t high = pow2((idx_t)(L - 1));
    return high | low;
}
