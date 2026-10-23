/*
 * A136119  Limiting sequence when we start with the positive integers
 *          (A000027) and delete in step n >= 1 the term at position n + a(n).
 * https://oeis.org/A136119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 45, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = ceil((n - 1/2) * sqrt(2))
 * (equivalently a(n) = floor((n - 1/2) * sqrt(2)) + 1)
 * This can be computed exactly using integer arithmetic:
 *   let k = 2*n - 1                      //  integer
 *   m = floor(k * sqrt(2)) = floor_sqrt(2 * k^2)
 *   a(n) = floor(m / 2) + 1
 *
 * This implementation assumes a definition of floor_sqrt(seq_t) in
 * "../utils/floor_sqrt.h". It works for any seq_t that can hold the
 * intermediate values for the requested n range.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    /* Undefined for n == 0, return 0 */
    if (n == 0) return 0;
    /* k = 2*n - 1, stays in idx_t; cast to seq_t for arithmetic. */
    idx_t k_idx = (idx_t)2 * n - (idx_t)1;
    seq_t k = (seq_t)k_idx;
    /* m = floor_sqrt(2 * k * k)  == floor(k * sqrt(2))          */
    seq_t m = floor_sqrt((seq_t)2 * k * k);
    /* a(n) = m/2 + 1 (integer division)                         */
    return (m / (seq_t)2) + (seq_t)1;
}
