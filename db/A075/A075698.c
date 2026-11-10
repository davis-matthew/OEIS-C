/*
 * A075698  Primes of index m from A075696, a(n) = prime(A075696(n)).
 * https://oeis.org/A075698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = prime(A075696(n)), where A075696(n) is the n-th k such
 *            that the decimal digit sum of k occurs as a substring of
 *            prime(k). Computed by a search over k
 * Range:     u8 n <= 9, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

static bool A075698_isprime(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    uint64_t k = 1;
    uint64_t p = 1;
    idx_t count = 0;
    while (1) {
        /* advance p to the k-th prime */
        do { p++; } while (!A075698_isprime(p));

        uint64_t s = 0, t = k;
        while (t) { s += t % 10; t /= 10; }

        char pb[32], sb[32];
        snprintf(pb, sizeof pb, "%llu", (unsigned long long)p);
        snprintf(sb, sizeof sb, "%llu", (unsigned long long)s);
        if (strstr(pb, sb) != NULL) {
            ++count;
            if (count == n) return (seq_t)p;
        }
        ++k;
    }
}
