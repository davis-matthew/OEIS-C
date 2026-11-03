/*
 * A002267  The 15 supersingular primes: primes dividing order of Monster
 *          simple group.
 * https://oeis.org/A002267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, nice
 * Formula:   search, not a closed form. Primes p = 2, 3 are always terms. A
 *            prime p >= 5 is a term (supersingular) iff every supersingular
 *            j-invariant in characteristic p lies in F_p. The total count is
 *            floor(p/12) + {0,1,1,2} for p == 1,5,7,11 (mod 12). The count
 *            lying in F_p is h(-4p)/2 for p == 1 (mod 4),...
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Validated: full, pass, 15 terms through n = 15 (2026-11-02)
 * Helpers:   is_prime_i64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_i64.h"

static long long a2267_gcd(long long x, long long y)
{
    if (x < 0) x = -x;
    if (y < 0) y = -y;
    while (y) { long long t = x % y; x = y; y = t; }
    return x;
}

/* class number h(D) for D < 0 (D = 0 or 1 mod 4), counting reduced primitive forms */
static long long a2267_h(long long D)
{
    long long d = -D, cnt = 0, a, b;
    for (a = 1; 3 * a * a <= d; a++) {
        for (b = -a + 1; b <= a; b++) {
            long long num = b * b + d, c;
            if (num % (4 * a) != 0) continue;
            c = num / (4 * a);
            if (c < a) continue;
            if (a == c && b < 0) continue;
            if (a2267_gcd(a2267_gcd(a, b), c) != 1) continue;
            cnt++;
        }
    }
    return cnt;
}

static bool a2267_supersingular(long long p)
{
    long long total, rational, r;
    if (p < 5) return true;
    r = p % 12;
    total = p / 12 + (r == 1 ? 0 : (r == 11 ? 2 : 1));
    if (p % 4 == 1)
        rational = a2267_h(-4 * p) / 2;
    else
        rational = (a2267_h(-p) + a2267_h(-4 * p)) / 2;
    return rational == total;
}

seq_t a(idx_t n)
{
    idx_t k = 0;
    long long p;
    for (p = 2; p <= 2000; p++) {
        if (!is_prime_i64(p)) continue;
        if (a2267_supersingular(p)) {
            k++;
            if (k == n) return (seq_t)p;
        }
    }
    return (seq_t)0;
}
