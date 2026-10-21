/*
 * A094510  Smallest number distinct from n, not occurring earlier and having
 *          in binary representation exactly as many ones as n.
 * https://oeis.org/A094510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the unique partner of n in the self‑inverse permutation
 *            that pairs numbers with the same binary popcount. For each
 *            popcount k, the sorted list of numbers with popcount k is paired
 *            consecutively: (a_0,a_1),(a_2,a_3),…
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   POPCOUNT64, n_choose_k, unrank_with_popcount
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/POPCOUNT64.h"
#include "../utils/n_choose_k.h"
#include "../utils/unrank_with_popcount.h"

/* population count for idx_t using a compiler builtin,
   with a portable fallback if needed. */
#if defined(__GNUC__) || defined(__clang__)
# define POPCOUNT64(n)   __builtin_popcountll(n)
#else

#endif

/* highest set bit position of a > 0; 0-based. */
static inline int msb_pos(idx_t n)
{
    return 63 - __builtin_clzll(n);
}

/* rank of n among numbers with popcount k (0‑based). */
static inline idx_t rank_with_popcount(idx_t n, unsigned k)
{
    idx_t rank = 0;
    unsigned ones = 0;
    int msb = msb_pos(n);
    for (int pos = msb; pos >= 0; --pos) {
        if ((n >> pos) & 1U) {
            idx_t remaining_bits = pos;
            unsigned needed = k - ones;
            if (needed <= remaining_bits)
                rank += n_choose_k(remaining_bits, needed);
            ones++;
        }
    }
    return rank;
}

/* required public function */
seq_t a(idx_t n)
{
    unsigned k = POPCOUNT64(n);
    idx_t r   = rank_with_popcount(n, k);
    idx_t partner_r = r ^ 1U;          /* toggle last bit of rank */
    return unrank_with_popcount(k, partner_r);
}
