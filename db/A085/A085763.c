/*
 * A085763  Number of palindromes between n and 2n (inclusive).
 * https://oeis.org/A085763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7441, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 115 terms through n = 9607 (2026-11-01)
 * Helpers:   reverse_digits_idx_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of palindromes between n and 2n (inclusive) */

#include <stdbool.h>
#include <limits.h>
#include "../utils/reverse_digits_idx_to_seq.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* avoid overflow of 2*n in very large indices */
    idx_t limit = (n <= (UINT64_MAX >> 1)) ? n + n : UINT64_MAX;

    seq_t cnt = 0;
    for (idx_t i = n; i <= limit; ++i) {
        if (reverse_digits_idx_to_seq(i) == (seq_t)i) ++cnt;
    }
    return cnt;
}
