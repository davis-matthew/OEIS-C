/*
 * A176409  Multiples of 3 or 7.
 * https://oeis.org/A176409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 110, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_multiples
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A176409 – multiples of 3 or 7
   a(1) = 0
   For n ≥ 2, a(n) is the smallest x ≥ 1 such that
            ⌊x/3⌋ + ⌊x/7⌋ – ⌊x/21⌋  ≥  n – 1
   This gives the nth non‑negative integer divisible by 3 or 7.
   The implementation uses a binary search on the count of such
   multiples, which runs in O(log n) time and works for all build
   types where idx_t and seq_t are unsigned integer types. */

#include <stdint.h>
#include "../utils/count_multiples.h"

seq_t a(idx_t n)
{
    if (n <= 1)          /* a(1) = 0 */
        return (seq_t)0;

    idx_t target = n - 1;          /* we look for target-th positive multiple */
    idx_t low = 1;
    idx_t high = 21 * target;      /* high is certainly large enough */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (count_multiples(mid) >= target)
            high = mid;
        else
            low = mid + 1;
    }

    return (seq_t)low;        /* low is the desired term a(n) for n ≥ 2 */
}
