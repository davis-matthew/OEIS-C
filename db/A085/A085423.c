/*
 * A085423  a(n) = floor(log_2(3n)).
 * https://oeis.org/A085423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(log_2(3*n)) (offset 1)
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   floor_log2_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t if needed */
#include "../utils/floor_log2_uint128.h"

seq_t a(idx_t n)
{
    /* n >= 1 as per problem statement */
    unsigned __int128 prod = (unsigned __int128)n * 3;
    return (seq_t)floor_log2_uint128(prod);
}
