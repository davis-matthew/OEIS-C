/*
 * A071830  Smallest k > n such that gpf(n) = gpf(k) where gpf = A006530
 *          (greatest prime factor).
 * https://oeis.org/A071830
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A071830: the smallest integer k > n such that gpf(n) = gpf(k) */
/* Searchforward with a direct factorisation routine. */
#include <stdint.h>
#include "../utils/gpf.h"

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;            /* undefined for n < 2 */

    uint64_t n64 = (uint64_t)n;
    uint64_t g   = gpf(n64);

    /* Prime or power of two → next multiple is 2*n. */
    if (g == n64 || g == 2) return (seq_t)(n64 * 2);

    /* Search forward for the next integer with the same greatest prime factor. */
    uint64_t k = n64 + 1;
    for (;;) {
        if (gpf(k) == g) return (seq_t)k;
        ++k;
    }
}
