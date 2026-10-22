/*
 * A001694  Powerful numbers, definition (1): if a prime p divides n then p^2
 *          must also divide n (also called squareful, square full,
 *          square-full or 2-powerful numbers).
 * https://oeis.org/A001694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = smallest x with N(x) >= n, where N(x) = Sum_{b
 *            squarefree, b^3 <= x} floor(sqrt(x/b^3)) counts powerful numbers
 *            <= x (each is uniquely a^2*b^3 with b squarefree); found by
 *            binary search
 * Range:     u8 n <= 25, u16 n <= 496, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   p1694_count, p1694_isqrt, p1694_sqfree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/p1694_count.h"
#include "../utils/p1694_isqrt.h"
#include "../utils/p1694_sqfree.h"

seq_t a(idx_t n) {
    uint64_t lo = 1, hi = (uint64_t)n * (uint64_t)n;
    if (hi < 1) hi = 1;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        if (p1694_count(mid) >= n) hi = mid;
        else lo = mid + 1;
    }
    return (seq_t)lo;
}
