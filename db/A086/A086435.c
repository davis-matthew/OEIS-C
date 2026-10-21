/*
 * A086435  Maximum number of parts possible in a factorization of n into a
 *          product of distinct numbers > 1.
 * https://oeis.org/A086435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum number of distinct factors > 1 whose product
 *            equals n. This is computed by recursively enumerating ascending
 *            factorisations and taking the longest one
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _max_parts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_max_parts.h"

/* Forward declaration */
static seq_t _max_parts(uint64_t n, uint64_t min_factor);

/* Public function with external linkage */
seq_t a(idx_t n) {
    /* the sequence is defined for n >= 1; a(1)=0 */
    if (n <= 1) return (seq_t)0;
    return _max_parts((uint64_t)n, (uint64_t)2);
}
