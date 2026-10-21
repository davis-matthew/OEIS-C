/*
 * A067587  Inverse of A066884 considered as a permutation of the positive
 *          integers.
 * https://oeis.org/A067587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(n) = binomial(w(n)+p(n),2) + p(n) + 1 * where w(n) =
 *            popcount(n) and p(n) = #{ m < n | popcount(m) = w(n) }
 * Range:     u8 n <= 66, u16 n <= 1410, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binom_small, count_lt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom_small.h"
#include "../utils/count_lt.h"

seq_t a(idx_t n) {
    unsigned int w = __builtin_popcountll((uint64_t)n);
    seq_t p = count_lt(n, w);
    seq_t total = (seq_t)w + p;               /* w + p */
    seq_t a_val = total * (total - (seq_t)1) / (seq_t)2 + p + (seq_t)1;
    return a_val;
}
