/*
 * A166935  A positive integer n is included if the longest palindromic
 *          substring (or at least one of which that is tied for longest) in
 *          the binary representation of n consists of identically valued
 *          digits (all 0's or all 1's).
 * https://oeis.org/A166935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n‑th positive integer such that its longest
 *            palindromic substring in binary representation consists only of
 *            identical bits (all 0's or all 1's). The sequence is generated
 *            by a simple search over successive integers
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   is_good_u64, longest_pal_len_u64, max_run_len_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_good_u64.h"
#include "../utils/longest_pal_len_u64.h"
#include "../utils/max_run_len_u64.h"

/* Public function defining the OEIS sequence. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t cand = 1;

    while (1) {
        if (is_good_u64(cand)) {
            ++count;
            if (count == n) {
                return (seq_t)cand;
            }
        }
        ++cand;
    }
}
