/*
 * A002796  Numbers that are divisible by each nonzero digit.
 * https://oeis.org/A002796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(n) = the n-th number that is divisible by each of its nonzero
 *            digits
 * Range:     u8 n <= 70, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 78 terms through n = 1990 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_self_dividing(seq_t num) {
    seq_t temp = num;
    while (temp > 0) {
        seq_t digit = temp % 10;
        if (digit != 0 && num % digit != 0) {
            return false;
        }
        temp /= 10;
    }
    return true;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t m = 0;
    
    while (count < n) {
        m++;
        if (is_self_dividing(m)) {
            count++;
        }
    }
    
    return m;
}
