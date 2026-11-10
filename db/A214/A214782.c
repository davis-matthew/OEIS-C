/*
 * A214782  Numbers n such that n-1 = A214781(n).
 * https://oeis.org/A214782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) = n-th number m such that the least k>=2 with m |
 *            Fibonacci(k-1)+2 equals m-1 (primes m only tested)
 * Range:     u8 n <= 15, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool A214782_prime(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

/* true iff smallest k>=2 with c | F(k-1)+2 is exactly c-1 */
static bool A214782_test(uint64_t c)
{
    if (c < 3) return false;
    uint64_t target = c - 2;
    uint64_t prev = 0, curr = 1 % c;
    for (uint64_t m = 1; m + 2 <= c; ++m) {
        if (curr == target) return m + 2 == c;
        uint64_t next = prev + curr;
        if (next >= c) next -= c;
        prev = curr;
        curr = next;
    }
    return false;
}

static uint64_t *A214782_cache = 0;
static size_t A214782_cnt = 0, A214782_cap = 0;
static uint64_t A214782_next = 2;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (A214782_cnt < (size_t)n) {
        uint64_t c = A214782_next++;
        if (A214782_prime(c) && A214782_test(c)) {
            if (A214782_cnt == A214782_cap) {
                A214782_cap = A214782_cap ? A214782_cap * 2 : 64;
                A214782_cache = (uint64_t *)realloc(A214782_cache, A214782_cap * sizeof(uint64_t));
            }
            A214782_cache[A214782_cnt++] = c;
        }
    }
    return (seq_t)A214782_cache[n - 1];
}
