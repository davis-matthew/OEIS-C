/*
 * A006049  Numbers k such that k and k+1 have the same number of distinct
 *          prime divisors.
 * https://oeis.org/A006049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   search: a(n) = n-th k with A001221(k) = A001221(k+1); omega
 *            found by trial division
 * Range:     u8 n <= 93, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static int count_distinct_primes(uint64_t m) {
    int count = 0;
    uint64_t i;
    if (m <= 1) return 0;
    if (m % 2 == 0) {
        count++;
        while (m % 2 == 0) m /= 2;
    }
    for (i = 3; i <= m / i; i += 2) {
        if (m % i == 0) {
            count++;
            while (m % i == 0) m /= i;
        }
    }
    if (m > 1) count++;
    return count;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t k = 1;
    int prev = count_distinct_primes(1);
    for (;;) {
        int cur = count_distinct_primes(k + 1);
        if (prev == cur) {
            count++;
            if (count == n) return (seq_t)k;
        }
        prev = cur;
        k++;
    }
}
