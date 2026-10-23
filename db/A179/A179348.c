/*
 * A179348  Number of permutations of 1..n with i-7<=p(i)<=i+6.
 * https://oeis.org/A179348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of a(n) for OEIS A179348
   (Number of permutations of 1..n with i-7 <= p(i) <= i+6)
   The function uses a dynamic programming with a sliding 14‑bit window.
   No hard‑coded values are used. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static const uint32_t W = 14u;                    /* window width 14 (=7 left + 6 right + 1) */
static const uint64_t FULL_MASK = ((uint64_t)1u << W) - 1u;   /* all bits set */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1; values below are undefined */

    /* allocate DP arrays */
    seq_t *dp    = alloc_seq_array(FULL_MASK + 1);
    seq_t *newdp = alloc_seq_array(FULL_MASK + 1);
    if (!dp || !newdp) {
        if (dp)    free_seq_array(dp);
        if (newdp) free_seq_array(newdp);
        return 0;
    }

    /* initial state for i = 1: values < 1 are considered already used */
    uint64_t initMask = 0;
    for (uint32_t k = 0; k < 7; ++k) initMask |= 1ULL << k;   /* bits 0..6 set for values -6..0 */
    dp[initMask] = (seq_t)1;

    /* process positions 1 .. n-1 */
    for (idx_t i = 1; i < n; ++i) {
        /* reset newdp to zero */
        for (uint32_t j = 0; j <= FULL_MASK; ++j) newdp[j] = 0;

        /* compute allowed values for position i */
        idx_t startVal = (i > 7) ? i - 7 : 1;
        idx_t endVal   = (i + 6 <= n) ? i + 6 : n;
        uint64_t allowed = 0;
        for (idx_t v = startVal; v <= endVal; ++v) {
            /* bit index t = v - (i-7) = v + 7 - i */
            uint32_t t = (uint32_t)(v + 7 - i);
            allowed |= 1ULL << t;
        }

        bool rightOut = (i + 7 > n);   /* new rightmost value is out of range */

        /* transition */
        for (uint32_t mask = 0; mask <= FULL_MASK; ++mask) {
            seq_t cnt = dp[mask];
            if (!cnt) continue;

            /* available (unused) allowed values */
            uint64_t avail = (~mask) & allowed & FULL_MASK;

            /* If the leftmost value is unused, it must be used now */
            if ((mask & 1ULL) == 0) {
                avail &= 1ULL;            /* only the leftmost bit */
                if (!avail) continue;     /* impossible state */
            }

            while (avail) {
                uint32_t t = __builtin_ctzll(avail);
                avail &= avail - 1;        /* clear lowest set bit */

                uint32_t usedMask = mask | (1U << t);        /* mark chosen value */
                uint32_t nextMask = usedMask >> 1;           /* slide window */
                if (rightOut) nextMask |= 1U << (W - 1);     /* new rightmost used */
                newdp[nextMask] += cnt;
            }
        }

        /* swap dp and newdp */
        seq_t *tmp = dp; dp = newdp; newdp = tmp;
    }

    /* final position n */
    idx_t startVal = (n > 7) ? n - 7 : 1;
    idx_t endVal   = n;
    uint64_t allowed = 0;
    for (idx_t v = startVal; v <= endVal; ++v) {
        uint32_t t = (uint32_t)(v + 7 - n);
        allowed |= 1ULL << t;
    }

    seq_t result = 0;
    for (uint32_t mask = 0; mask <= FULL_MASK; ++mask) {
        seq_t cnt = dp[mask];
        if (!cnt) continue;

        uint64_t avail = (~mask) & allowed & FULL_MASK;
        /* leftmost value must be used if still unused */
        if ((mask & 1ULL) == 0) {
            avail &= 1ULL;
            if (!avail) continue;
        }

        /* each available choice completes a permutation */
        while (avail) {
            avail &= avail - 1;
            result += cnt;
        }
    }

    free_seq_array(dp);
    free_seq_array(newdp);
    return result;
}
