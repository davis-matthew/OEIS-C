/*
 * A004144  Nonhypotenuse numbers (indices of positive squares that are not
 *          the sums of 2 distinct nonzero squares).
 * https://oeis.org/A004144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 129, u16 n <= 9161, u32 n <= 9161, u64 n <= 9161,
 *            u128 n <= 9161, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the n-th positive integer with no prime factors of form 4k+1 */

#include <stdbool.h>

static bool is_nonhypotenuse(seq_t m) {
    seq_t temp = m;
    
    while (temp % 2 == 0) {
        temp /= 2;
    }
    
    for (seq_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            if (p % 4 == 1) {
                return false;
            }
            while (temp % p == 0) {
                temp /= p;
            }
        }
    }
    
    if (temp > 1 && temp % 4 == 1) {
        return false;
    }
    
    return true;
}

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t m = 0;
    
    while (count < (seq_t)n) {
        m++;
        if (is_nonhypotenuse(m)) {
            count++;
        }
    }
    
    return m;
}
