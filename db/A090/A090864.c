/*
 * A090864  Complement of generalized pentagonal numbers (A001318).
 * https://oeis.org/A090864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th positive integer that is NOT a generalized
 *            pentagonal number * (complement of A001318). The implementation
 *            scans the natural numbers * and counts those that are not of the
 *            form k(3k±1)/2 for some integer k≥1
 * Range:     u8 n <= 229, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_gpn
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_gpn.h"

seq_t a(idx_t n) {
    /* Returns the n-th term (1‑based) of A090864. */
    if (n == 0)
        return (seq_t)0;  /* undefined by OEIS, but safe fallback. */

    idx_t count = 0;
    seq_t candidate = (seq_t)1;

    for (;;) {
        if (!is_gpn(candidate)) {
            ++count;
            if (count == n)
                return candidate;
        }
        ++candidate;
    }
}
