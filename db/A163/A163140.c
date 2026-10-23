/*
 * A163140  Integers n such that in base 2, 3 `1`'s for each `0`.
 * https://oeis.org/A163140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 1554, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   nchoosek
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/nchoosek.h"
/* Sequence A163140: integers whose binary representation contains
   exactly three 1-bits for each 0-bit.  The sequence is given by
   sorting all binary strings of length 4k (k ≥ 1) that start with 1
   and contain k zeros (hence 3k ones).  The function returns a(n)
   for an arbitrary index n ≥ 1.  It uses combinatorial unranking. */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1, behaviour below is irrelevant */

    idx_t group_start = 1, k = 1;
    while (1) {
        idx_t group_cnt = nchoosek(4 * k - 1, k);
        if (n <= group_start + group_cnt - 1) {
            idx_t r = n - group_start;              /* 0‑based rank in this group */
            idx_t length = 4 * k;                   /* total number of bits */
            idx_t zeros_left = k;
            seq_t val = 1;                          /* leading 1 */

            for (idx_t pos = 1; pos < length; ++pos) {
                idx_t rem = (length - 1) - pos;     /* remaining positions after this one */
                if (zeros_left > 0) {
                    idx_t cnt0 = nchoosek(rem, zeros_left - 1);
                    if (r < cnt0) {                /* set this bit to 0 */
                        val = (val << 1);
                        --zeros_left;
                    } else {                       /* set it to 1 */
                        r -= cnt0;
                        val = (val << 1) | 1;
                    }
                } else {                           /* all remaining bits are 1 */
                    val = (val << 1) | 1;
                }
            }
            return val;
        }
        group_start += group_cnt;
        ++k;
    }
}
