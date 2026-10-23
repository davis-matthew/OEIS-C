/*
 * A045030  Numbers whose base-4 representation contains exactly three 0's and
 *          two 1's.
 * https://oeis.org/A045030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3344, u32 n <= 5700, u64 n <= 5700,
 *            u128 n <= 5700, big any n
 * Helpers:   has_three_zeroes_two_ones
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A045030:
 *  Numbers whose base-4 representation contains exactly three 0's and two 1's.
 *  The function generates the n‑th such number by brute‑force enumeration
 *  of integers in ascending order until the required count is reached.
 */

#include <stdbool.h>
#include "../utils/has_three_zeroes_two_ones.h"

/* Return the n‑th term of the sequence (a(1) = 257). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1, undefined for n==0 */

    idx_t  count      = 0;
    seq_t  candidate  = (seq_t)1;

    while (count < n) {
        if (has_three_zeroes_two_ones(candidate)) {
            ++count;
            if (count == n) return candidate;
        }
        ++candidate;
    }
    /* unreachable, but required for completeness */
    return (seq_t)0;
}
