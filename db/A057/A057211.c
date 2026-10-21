/*
 * A057211  Alternating runs of ones and zeros, where the n-th run has length
 *          n.
 * https://oeis.org/A057211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = round(sqrt(2*n)) mod 2 (equivalently, a(n) =
 *            (1-(-1)^round(sqrt(2*n)))/2)
 * Range:     u8 n <= 7875, u16 n <= 7875, u32 n <= 7875, u64 n <= 7875,
 *            u128 n <= 7875, big any n
 * Helpers:   floor_sqrt_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt_uint128.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* offset is 1; value below offset is undefined */

    /* compute t = 2*n, using 128-bit to avoid overflow */
    __int128 t = (__int128)n << 1;

    /* floor sqrt of t */
    uint64_t k = floor_sqrt_uint128(t);

    /* determine rounded sqrt: round up if t > k*k + k */
    __int128 kk = (__int128)k * k;
    uint64_t m = k + (t > kk + k);

    /* result is parity of m */
    return (seq_t)(m & 1);
}
