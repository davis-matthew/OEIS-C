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
 * Validated: standard, pass, 70 terms through n = 9422 (2026-11-01)
 * Helpers:   is_squarefree_u64, isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_squarefree_u64.h"
#include "../utils/isqrt_u64.h"

static uint64_t p1694_count(uint64_t x) {
    uint64_t total = 0, b;
    for (b = 1; b * b * b <= x; b++)
        if (is_squarefree_u64(b))
            total += isqrt_u64(x / (b * b * b));
    return total;
}

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
