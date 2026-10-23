/*
 * A199976  Multiplicative digital root of n-th nonprime number.
 * https://oeis.org/A199976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = multiplicative digital root of the nth nonprime number
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

static seq_t multiplicative_root(seq_t x) {
    if (x == 0) return 0;
    seq_t y = x;
    while (y >= 10) {
        seq_t prod = 1;
        seq_t tmp = y;
        while (tmp > 0) {
            seq_t digit = tmp % 10;
            prod *= digit;
            if (prod == 0) break;
            tmp /= 10;
        }
        y = prod;
    }
    return y;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t candidate = 1;
    while (count < n) {
        if (!is_prime(candidate)) {
            ++count;
            if (count == n) {
                seq_t val = (seq_t)candidate;
                return multiplicative_root(val);
            }
        }
        ++candidate;
    }
    return 0;  /* unreachable */
}
