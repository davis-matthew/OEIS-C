/*
 * A071925  Digitally balanced numbers: binary numbers which have the same
 *          number of 0's as 1's; decimal representation: A031443.
 * https://oeis.org/A071925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = decimal representation of the n-th binary string (in
 *            increasing length, lexicographic order) that has an equal number
 *            of 0’s and 1’s and starts with 1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 175, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   choose_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/choose_uint64.h"

seq_t a(idx_t n)
{
    /* Find the minimal even length L = 2*m such that the cumulative
       count of balanced strings of length up to L is >= n. */
    uint64_t cumulative = 0;
    uint64_t m = 0;               /* half of the length, m >= 1 */
    uint64_t count_len = 0;       /* C(2*m-1, m-1) for the current length */

    while (cumulative < (uint64_t)n) {
        ++m;
        count_len = choose_uint64(2 * m - 1, m - 1);
        cumulative += count_len;
    }

    uint64_t prev_cumulative = cumulative - count_len;
    uint64_t rank = (uint64_t)n - prev_cumulative - 1;  /* 0‑based */

    uint64_t len = 2 * m;              /* total length of the string */
    seq_t val = 1;                     /* first bit is always 1 */

    uint64_t ones_needed = m - 1;      /* remaining ones to place */
    uint64_t r = rank;                 /* rank within this length block */

    for (uint64_t pos = 1; pos < len; ++pos) {
        uint64_t remaining = len - 1 - pos;              /* positions after current */
        uint64_t count_zero = 0;
        if (ones_needed <= remaining) {
            count_zero = choose_uint64(remaining, ones_needed);
        }
        if (r < count_zero) {
            /* bit is 0 */
            val = val * 10;
        } else {
            /* bit is 1 */
            val = val * 10 + 1;
            r -= count_zero;
            --ones_needed;
        }
    }

    return (seq_t)val;
}
