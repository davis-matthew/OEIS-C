/*
 * A162518  Characteristic function of magic numbers A018226: 1 if n is a
 *          magic number else 0.
 * https://oeis.org/A162518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is a magic number (2, 8, 20, 28, 50, 82, 126),
 *            otherwise 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Known magic numbers. */
    static const idx_t magic[] = {2ULL, 8ULL, 20ULL, 28ULL, 50ULL, 82ULL, 126ULL};
    const size_t len = sizeof(magic) / sizeof(magic[0]);

    /* Binary search over the sorted list. */
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (n == magic[mid]) return (seq_t)1;
        if (n < magic[mid]) hi = mid;
        else lo = mid + 1;
    }
    return (seq_t)0;
}
