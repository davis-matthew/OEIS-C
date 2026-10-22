/*
 * A001602  Fibonacci entry points: a(n) = smallest m > 0 such that the n-th
 *          prime divides Fibonacci(m).
 * https://oeis.org/A001602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = A001177(prime(n)) = smallest m > 0 with prime(n) |
 *            Fibonacci(m); found by stepping Fibonacci numbers mod prime(n)
 *            (a(n) <= prime(n)+1)
 * Range:     u8 n <= 57, u16 n <= 6543, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *a001602_primes = NULL;
static uint64_t a001602_count = 0;

static void a001602_sieve(uint64_t need) {
    uint64_t limit = 64;
    while (a001602_count < need) {
        limit *= 2;
        unsigned char *comp = (unsigned char *)calloc((size_t)(limit + 1), 1);
        uint64_t cnt = 0;
        for (uint64_t i = 2; i <= limit; i++) {
            if (!comp[i]) {
                cnt++;
                for (uint64_t j = i * i; j <= limit; j += i) comp[j] = 1;
            }
        }
        if (cnt >= need) {
            free(a001602_primes);
            a001602_primes = (uint64_t *)malloc((size_t)cnt * sizeof(uint64_t));
            uint64_t k = 0;
            for (uint64_t i = 2; i <= limit; i++)
                if (!comp[i]) a001602_primes[k++] = i;
            a001602_count = cnt;
        }
        free(comp);
    }
}

seq_t a(idx_t n) {
    a001602_sieve((uint64_t)n);
    uint64_t p = a001602_primes[n - 1];

    uint64_t fib_prev = 0;
    uint64_t fib_curr = 1 % p;
    for (uint64_t m = 1; m <= p + 1; m++) {
        if (fib_curr == 0) return (seq_t)m;
        uint64_t fib_next = (fib_prev + fib_curr) % p;
        fib_prev = fib_curr;
        fib_curr = fib_next;
    }
    return (seq_t)0;
}
