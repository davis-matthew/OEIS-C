/*
 * A031964  Numbers with exactly four distinct base-5 digits.
 * https://oeis.org/A031964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer whose base‑5 representation *
 *            contains exactly four distinct digits. The algorithm counts how
 *            many * such n‑digit numbers there are for each length, then
 *            finds the * k‑th number of the appropriate length by
 *            constructing its base‑5 * digits from most to least signi...
 * Range:     u8 n <= 24, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   _count_len, _pow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_count_len.h"
#include "../utils/_pow_idx.h"

/* small binomial coefficients up to 5 */
static const int _C[6][6] = {
    {1,1,1,1,1,1},
    {1,1,1,1,1,1},
    {1,2,1,1,1,1},
    {1,3,3,1,1,1},
    {1,4,6,4,1,1},
    {1,5,10,10,5,1}
};

/* Count of completions of the suffix of length 'rem' when the current
 * used‑digit mask is 'mask'.  The mask is a 5‑bit bitset indicating
 * which base‑5 digits have already appeared.  The function returns the
 * number of ways to choose the remaining digits so that the final
 * number uses exactly four distinct digits. */
static idx_t _count_suffix(int mask, idx_t rem) {
    int used = __builtin_popcount((unsigned)mask);
    if (used > 4) return 0;
    if (rem == 0) return (used == 4) ? 1 : 0;

    if (used == 4) /* already have all four digits */
        return _pow_idx(4, rem);

    int need = 4 - used;                 /* new digits still required */
    int missing_mask = (~mask) & 0x1F;    /* available digits that are missing */
    int miss_cnt = __builtin_popcount((unsigned)missing_mask);
    if (need > miss_cnt) return 0;

    /* Ways to choose which missing digits will become the final four. */
    int ways = _C[miss_cnt][need];

    /* Count strings of length rem over these 4 digits such that each of
     * the 'need' missing digits appears at least once.  Inclusion–exclusion
     * over the subset of missing digits (only the missing ones matter,
     * already used digits have no restriction). */
    idx_t F = 0;
    for (int j = 0; j <= need; ++j) {
        idx_t term = _pow_idx(4 - j, rem);
        if ((j & 1) == 0)
            F += (idx_t)_C[need][j] * term;
        else
            F -= (idx_t)_C[need][j] * term;
    }

    return (idx_t)ways * F;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* safety guard, offset is 1 */

    /* Find the length of the n‑th number. */
    idx_t len = 4;
    idx_t cum = 0;
    while (1) {
        idx_t cnt = _count_len(len);
        if (cum + cnt >= n) break;
        cum += cnt;
        ++len;
    }
    idx_t k = n - cum; /* 1‑based index within length‑len numbers */

    seq_t result = 0;
    int mask = 0; /* digits already used */

    for (int pos = 0; pos < len; ++pos) {
        int start = (pos == 0) ? 1 : 0;
        for (int d = start; d <= 4; ++d) {
            int newmask = mask | (1 << d);
            idx_t rem = len - pos - 1;
            idx_t cnt = _count_suffix(newmask, rem);
            if (k > cnt) {
                k -= cnt;
            } else {
                result = result * 5 + (seq_t)d;
                mask = newmask;
                break;
            }
        }
    }
    return result;
}
