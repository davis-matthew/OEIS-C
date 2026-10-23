/*
 * A183137  a(n) = [1/s] + [2/s] + ... + [n/s], where s = (golden ratio)^2 =
 *          (3+sqrt(5))/2 and [] = floor.
 * https://oeis.org/A183137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [1/s] + [2/s] + ... + [n/s] with s = (3+sqrt(5))/2 (the
 *            golden ratio squared)
 * Range:     u8 n <= 37, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   can_inc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Author: ChatGPT */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/can_inc.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t m = 0; /* floor(k/s) for current k */

    for (idx_t k = 1; k <= n; ++k) {
        /* Increment m while k >= (m+1)*s. */
        while (can_inc(k, m)) ++m;
        sum += (seq_t)m;
    }

    return sum;
}
