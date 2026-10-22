/*
 * A002381  Numbers of the form (p^2 - 1)/120 where p is 1 or prime.
 * https://oeis.org/A002381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p^2 - 1)/120 where p ranges over 1 and odd primes p with
 *            p != 3 and p ≡ 1, 4 (mod 5)
 * Range:     u8 n <= 2, u16 n <= 26, u32 n <= 3548, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    if (n == 1) return 0;
    
    uint64_t limit = 50 * n;
    if (limit < 1000) limit = 1000;
    if (limit > 10000000) limit = 10000000;
    
    bool *is_prime = (bool *)malloc(limit + 1);
    memset(is_prime, true, limit + 1);
    is_prime[0] = is_prime[1] = false;
    
    for (uint64_t i = 2; i * i <= limit; i++) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= limit; j += i) {
                is_prime[j] = false;
            }
        }
    }
    
    idx_t count = 1;
    for (uint64_t p = 2; p <= limit; p++) {
        if (is_prime[p]) {
            if (p != 3 && (p % 5 == 1 || p % 5 == 4)) {
                count++;
                if (count == n) {
                    free(is_prime);
                    seq_t ps = (seq_t)p;
                    seq_t p_sq = ps * ps;
                    return (p_sq - 1) / 120;
                }
            }
        }
    }
    
    free(is_prime);
    return 0;
}
