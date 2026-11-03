/*
 * A007436  Moebius transform of Fibonacci numbers.
 * https://oeis.org/A007436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} μ(n/d) * F_d, where μ is Möbius and F is
 *            Fibonacci
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 186,
 *            big any n
 * Validated: standard, pass, 56 terms through n = 4596 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *get_divisors(uint64_t n, idx_t *count) {
    uint64_t *divisors = (uint64_t *)malloc(sizeof(uint64_t) * 256);
    *count = 0;
    
    for (uint64_t i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            divisors[(*count)++] = i;
            if (i != n / i) {
                divisors[(*count)++] = n / i;
            }
        }
    }
    
    return divisors;
}

static int moebius(uint64_t n) {
    if (n == 1) return 1;
    
    int prime_count = 0;
    uint64_t temp = n;
    
    if (temp % 2 == 0) {
        prime_count++;
        temp /= 2;
        if (temp % 2 == 0) return 0;
    }
    
    for (uint64_t i = 3; i * i <= temp; i += 2) {
        if (temp % i == 0) {
            prime_count++;
            temp /= i;
            if (temp % i == 0) return 0;
        }
    }
    
    if (temp > 1) prime_count++;
    
    return (prime_count % 2 == 0) ? 1 : -1;
}

static seq_t fibonacci(idx_t n) {
    if (n == 1 || n == 2) return 1;
    seq_t a = 1, b = 1;
    for (idx_t i = 3; i <= n; i++) {
        seq_t c = a + b;
        a = b;
        b = c;
    }
    return b;
}

seq_t a(idx_t n) {
    idx_t div_count;
    uint64_t *divisors = get_divisors(n, &div_count);
    
    seq_t pos = 0, neg = 0;
    for (idx_t i = 0; i < div_count; i++) {
        uint64_t d = divisors[i];
        int mu = moebius(n / d);
        seq_t fib_d = fibonacci((idx_t)d);
        if (mu == 1) {
            pos += fib_d;
        } else if (mu == -1) {
            neg += fib_d;
        }
    }
    
    free(divisors);
    
    return pos - neg;
}
