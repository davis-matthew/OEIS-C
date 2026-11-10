/*
 * A089534  A089348 indexed by A000040.
 * https://oeis.org/A089534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that prime(k) = A089348(n), where A089348(n) is
 *            the smallest prime 10*n*m+1 with m >= 1; computed by search plus
 *            a sieve count of primes <= that prime
 * Range:     u8 n <= 35, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool A089534_isprime(uint64_t x)
{
    uint64_t d;
    if (x < 2) return false;
    if (x < 4) return true;
    if (x % 2 == 0) return false;
    for (d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    uint64_t p = 0, m, i, j, cnt = 0;
    unsigned char *comp;

    if (n < 1) return (seq_t)0;

    for (m = 1;; m++) {
        uint64_t c = 10 * nn * m + 1;
        if (A089534_isprime(c)) { p = c; break; }
    }

    comp = (unsigned char *)calloc((size_t)p + 1, 1);
    if (!comp) return (seq_t)0;
    for (i = 2; i * i <= p; i++)
        if (!comp[i])
            for (j = i * i; j <= p; j += i)
                comp[j] = 1;
    for (i = 2; i <= p; i++)
        if (!comp[i]) cnt++;
    free(comp);
    return (seq_t)cnt;
}
