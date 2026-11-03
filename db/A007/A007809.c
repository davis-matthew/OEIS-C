/*
 * A007809  Smallest prime with n distinct digits.
 * https://oeis.org/A007809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, base, full
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Validated: full, pass, 9 terms through n = 9 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest prime with n distinct digits (search algorithm) */

#include <stdbool.h>
#include "../utils/is_prime_u64.h"

static int count_distinct_digits(seq_t num) {
    bool seen[10] = {false};
    seq_t temp = num;
    while (temp > 0) {
        seen[(int)(temp % 10)] = true;
        temp /= 10;
    }
    int count = 0;
    for (int i = 0; i < 10; i++) {
        if (seen[i]) count++;
    }
    return count;
}

seq_t a(idx_t n) {
    if (n < 1 || n > 10) return 0;
    
    static seq_t cache[11] = {0};
    static bool computed[11] = {false};
    
    int ni = (int)n;
    if (computed[ni]) return cache[ni];
    
    seq_t start;
    long long max_search;
    
    if (n == 1) {
        start = 2;
        max_search = 100;
    } else if (n == 2) {
        start = 10;
        max_search = 1000;
    } else if (n == 3) {
        start = 102;
        max_search = 10000;
    } else if (n == 4) {
        start = 1023;
        max_search = 100000;
    } else if (n == 5) {
        start = 10234;
        max_search = 1000000;
    } else if (n == 6) {
        start = 102345;
        max_search = 10000000;
    } else if (n == 7) {
        start = 1023456;
        max_search = 100000000;
    } else if (n == 8) {
        start = 10234567;
        max_search = 1000000000;
    } else if (n == 9) {
        start = 102345678;
        max_search = 10000000000LL;
    } else {
        start = 1023456789;
        max_search = 100000000000LL;
    }
    
    for (seq_t current = start; (long long)current < (long long)start + max_search; current++) {
        if (is_prime_u64((long long)current) && count_distinct_digits(current) == ni) {
            computed[ni] = true;
            cache[ni] = current;
            return current;
        }
    }
    
    return 0;
}
