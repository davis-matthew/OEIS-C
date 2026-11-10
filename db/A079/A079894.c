/*
 * A079894  a(n) = gcd(A079890(n), A079892(n)).
 * https://oeis.org/A079894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A079890(n), A079892(n)); both found by forward search
 *            with trial-division factor counts
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Validated: partial, pass, 94 terms through n = 94 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static void A079894_counts(uint64_t x, unsigned *big, unsigned *dist)
{
    unsigned b = 0, d = 0;
    uint64_t p;
    for (p = 2; p * p <= x; p += (p == 2 ? 1 : 2)) {
        if (x % p == 0) {
            d++;
            while (x % p == 0) {
                x /= p;
                b++;
            }
        }
    }
    if (x > 1) {
        b++;
        d++;
    }
    *big = b;
    *dist = d;
}

static uint64_t A079894_gcd(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    unsigned bn, dn, b, d;
    uint64_t m, r1 = 0, r2 = 0;
    A079894_counts(N, &bn, &dn);
    for (m = N + 1; ; m++) {
        A079894_counts(m, &b, &d);
        if (!r1 && b == bn + 1) r1 = m;
        if (!r2 && d == dn + 1) r2 = m;
        if (r1 && r2) break;
    }
    return (seq_t)A079894_gcd(r1, r2);
}
