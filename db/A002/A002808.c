/*
 * A002808  The composite numbers: numbers n of the form x*y for x > 1 and y >
 *          1.
 * https://oeis.org/A002808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, core
 * Formula:   a(n) = n-th composite number (positive integers that are neither
 *            1 nor prime)
 * Range:     u8 n <= 200, u16 n <= 9120, u32 n <= 9120, u64 n <= 9120,
 *            u128 n <= 9120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>


static bool is_prime(seq_t num) {
    if (num <= 1) return false;
    if (num == 2) return true;
    if (num % 2 == 0) return false;
    if (num == 3) return true;
    if (num % 3 == 0) return false;
    
    for (seq_t i = 5; i <= num / i; i += 6) {
        if (num % i == 0 || num % (i + 2) == 0)
            return false;
    }
    return true;
}

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t candidate = 4;
    seq_t target = (seq_t)n;
    
    while (true) {
        if (!is_prime(candidate)) {
            count++;
            if (count == target) return candidate;
        }
        candidate++;
    }
}
