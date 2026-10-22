/*
 * A006241  Number of minimal plane trees with n terminal nodes.
 * https://oeis.org/A006241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{1 != d | n} a(n / d)^d Base cases:
 *            a(1)=a(2)=a(3)=a(5)=a(7)=1, a(4)=2, a(6)=3
 * Range:     u8 n <= 23, u16 n <= 47, u32 n <= 95, u64 n <= 167,
 *            u128 n <= 335, big any n
 * Helpers:   a000248_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/a000248_pow.h"

#define CACHE_SIZE 10001

static seq_t cache[CACHE_SIZE];
static bool computed[CACHE_SIZE];

static seq_t compute_a(idx_t n);

static seq_t compute_a(idx_t n) {
    if (n < CACHE_SIZE && computed[n]) {
        return cache[n];
    }
    
    if (n == 1) return 1;
    if (n == 2) return 1;
    if (n == 3) return 1;
    if (n == 4) return 2;
    if (n == 5) return 1;
    if (n == 6) return 3;
    if (n == 7) return 1;
    
    seq_t result = 0;
    
    for (idx_t d = 2; d * d <= n; d++) {
        if (n % d == 0) {
            seq_t term = a000248_pow(compute_a(n / d), d);
            result += term;
            
            idx_t other = n / d;
            if (other != d) {
                term = a000248_pow(compute_a(d), other);
                result += term;
            }
        }
    }
    
    result += 1;
    
    if (n < CACHE_SIZE) {
        cache[n] = result;
        computed[n] = true;
    }
    
    return result;
}

seq_t a(idx_t n) {
    return compute_a(n);
}
