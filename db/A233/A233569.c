/*
 * A233569  Canonical parts power representation of n: n =
 *          concatenation((1)^k_1,(10)^k_2,...).
 * https://oeis.org/A233569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 2499, u32 n <= 2499, u64 n <= 2499,
 *            u128 n <= 2499, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Canonical parts power representation of n.
   For each run of a '1' followed by k zeros in the binary form of n,
   we form the block (10…0) with k zeros.  The canonical representation
   is the concatenation of all 0‑zero blocks, then all 1‑zero blocks,
   then all 2‑zero blocks, etc.  This function computes that value
   for any n ≥ 0.                                     */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)0;

    /* Count how many blocks have k zeros (k = 0 … 63). */
    uint64_t part_cnt[64] = {0};
    int msb = 0;
    idx_t tmp = n;
    while (tmp) { msb++; tmp >>= 1; }

    int i = msb - 1;                 /* current bit index */
    while (i >= 0) {
        if ( ((n >> i) & (idx_t)1) != 0 ) {   /* start of a block */
            int zeros = 0;
            int j = i - 1;
            while (j >= 0 && ( ((n >> j) & (idx_t)1) == 0 )) {
                zeros++;
                j--;
            }
            part_cnt[zeros]++;     /* record this block */
            i = j;                 /* continue scanning from j */
        } else {
            i--;                   /* skip a zero that doesn't start a block */
        }
    }

    /* Build the canonical number. */
    unsigned __int128 acc = 0;
    for (int k = 0; k < 64; ++k) {
        uint64_t cnt = part_cnt[k];
        for (uint64_t t = 0; t < cnt; ++t) {
            acc = (acc << (k + 1)) | ( ((unsigned __int128)1) << k );
        }
    }
    return (seq_t)acc;
}
