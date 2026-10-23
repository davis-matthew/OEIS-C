/*
 * A031447  Numbers whose base-2 representation has 4 more 0's than 1's.
 * https://oeis.org/A031447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   _binom_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_binom_uint64.h"
/* This function generates the OEIS sequence A031447
   (numbers whose binary representation has 4 more 0’s than 1’s) by
   iterating over the possible bit‑lengths and counting the numbers
   with the required balance of 0’s and 1’s.  For a given bit‑length
   L = 2*k + 4 (k = total number of 1's, k ≥ 1) the leading bit is
   forced to be 1, and the remaining k-1 ones are chosen among the
   L-1 remaining positions.  The numbers are produced in ascending
   order by mapping a 0‑based rank to a binary mask using the
   combinatorial number system. */

seq_t a(idx_t n) {
    if (n == 0) return 0; /* sequence is 1‑based; return 0 for safety */

    uint64_t cumulative = 0;

    /* iterate over k = number of 1's in the binary representation
       (including the leading 1) */
    for (uint64_t k = 1; ; ++k) {
        uint64_t L = 2 * k + 4;                     /* total bit length   */
        uint64_t block = _binom_uint64(L - 1, k - 1); /* how many numbers of this length */

        if (cumulative + block >= n) {
            uint64_t rank = n - cumulative - 1;     /* 0‑based rank within the block */
            seq_t mask = 0;
            uint64_t rem  = k - 1;                   /* remaining 1's to place */

            /* reconstruct the binary mask from the rank using
               combinatorial number system; iterate over positions
               from the highest down to 0. */
            for (int64_t pos = (int64_t)L - 2; pos >= 0 && rem > 0; --pos) {
                uint64_t cnt0 = _binom_uint64(pos, rem);
                if (rank >= cnt0) {
                    mask |= ((seq_t)1 << pos);
                    rank -= cnt0;
                    --rem;
                }
            }
            /* prepend the leading 1 at position L-1 */
            return ((seq_t)1 << (L - 1)) | mask;
        }
        cumulative += block;
    }
}
