/*
 * A000415  Numbers that are the sum of 2 but no fewer nonzero squares.
 * https://oeis.org/A000415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   Nonsquares that can be represented as sum of 2 squares
 * Range:     u8 n <= 81, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a93_isqrt_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/a93_isqrt_big.h"

static bool is_perfect_square(seq_t n) {
    seq_t root = a93_isqrt_big(n);
    return root * root == n;
}

static bool can_be_sum_of_two_squares(seq_t n) {
    seq_t temp = n;
    
    while (temp % 2 == 0) {
        temp /= 2;
    }
    
    seq_t limit = a93_isqrt_big(temp);
    for (seq_t p = 3; p <= limit; p += 2) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                exp++;
                temp /= p;
            }
            if ((p % 4 == 3) && (exp % 2 == 1)) {
                return false;
            }
        }
    }
    
    if (temp > 1 && temp % 4 == 3) {
        return false;
    }
    
    return true;
}

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t k = 1;
    
    while (count < (seq_t)n) {
        k++;
        if (!is_perfect_square(k) && can_be_sum_of_two_squares(k)) {
            count++;
        }
    }
    
    return k;
}
