/*
 * A163399  a(n) = (the binary order of n)^sigma(n).
 * https://oeis.org/A163399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(log2(n))^sigma(n)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 23,
 *            big any n
 * Helpers:   big_pow, floor_log2_uint64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_log2_uint64.h"
#include "../utils/sigma_of.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    seq_t base;
    if (n == 1) {
        base = (seq_t)0;
    } else {
        /* ceil(log2(n)) == floor_log2_uint64(n-1) + 1 */
        base = (seq_t)(floor_log2_uint64(n - 1) + 1);
    }
    seq_t sigma = sigma_of(n);
    return big_pow(base, sigma);
}
