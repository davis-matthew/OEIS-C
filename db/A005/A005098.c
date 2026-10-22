/*
 * A005098  Numbers k such that 4k + 1 is prime.
 * https://oeis.org/A005098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = (A002144(n)-1)/4; computed by searching k = 1, 2, ... and
 *            counting those with 4k+1 prime
 * Range:     u8 n <= 83, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t k = 0;

    while (count < n) {
        k++;
        if (is_prime(4 * k + 1)) {
            count++;
        }
    }

    return (seq_t)k;
}
