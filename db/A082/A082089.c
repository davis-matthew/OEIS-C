/*
 * A082089  a(n)-th prime is the fixed point if function A008472 is iterated
 *          when started at factorial of n-th prime.
 * https://oeis.org/A082089
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi(A082087(p(n-1)!)), where A082087 iterates sopf =
 *            A008472 to its fixed point, starting from sopf(p!) = sum of
 *            primes <= p. The b-file has a(2) = 1 for 2!, so index n uses
 *            prime p(n-1)
 * Range:     u8 n <= 60, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 92 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t A082089_sopf(uint64_t m)
{
    uint64_t s = 0, p;
    if (m < 2) return 0;
    for (p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            s += p;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) s += m;
    return s;
}

static bool A082089_isprime(uint64_t x)
{
    uint64_t d;
    if (x < 2) return false;
    for (d = 2; d * d <= x; d++)
        if (x % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    uint64_t cnt = 0, p = 1, sum = 0, x, y, i, j, pi;
    uint64_t target;
    unsigned char *comp;

    if (n < 2) return 0;
    target = (uint64_t)n - 1;

    while (cnt < target) {
        p++;
        if (A082089_isprime(p)) cnt++;
    }
    /* sopf(p!) = sum of primes <= p */
    for (i = 2; i <= p; i++)
        if (A082089_isprime(i)) sum += i;

    x = sum;
    for (;;) {
        y = A082089_sopf(x);
        if (y == x) break;
        x = y;
    }

    comp = (unsigned char *)calloc((size_t)x + 1, 1);
    if (!comp) return 0;
    pi = 0;
    for (i = 2; i <= x; i++) {
        if (!comp[i]) {
            pi++;
            for (j = i * i; j <= x; j += i) comp[j] = 1;
        }
    }
    free(comp);
    return (seq_t)pi;
}
