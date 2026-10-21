/*
 * A088659  a(n) = n*(p-1) where p is the largest prime factor of n.
 * https://oeis.org/A088659
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(p-1) where p is the largest prime factor of n (offset
 *            2: a(2)=2)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _lpf_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_lpf_uint64.h"

seq_t a(idx_t n) {
    if (n < 2) return 0;            /* behaviour below offset is irrelevant */
    seq_t p = _lpf_uint64((uint64_t)n);   /* largest prime factor */
    return (seq_t)n * (p - 1);
}
