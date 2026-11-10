/*
 * A082883  Primes p(x) satisfying the following conditions: (a) A082882(x)=1;
 *          (b) {p(x),p(x+1)} are not twin primes; (c) values of A075860(j)
 *          for j composites between these two non-twin primes are identical.
 * https://oeis.org/A082883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- a(n) = n-th prime p whose next prime q > p+2 and
 *            A075860(j) is the same for every composite j, p<j<q
 * Range:     u8 n <= 1, u16 n <= 18, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 34 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

static uint64_t A082883_sumdpf(uint64_t x) {
    uint64_t sum = 0;
    if ((x & 1ULL) == 0) {
        sum += 2;
        while ((x & 1ULL) == 0) x >>= 1;
    }
    for (uint64_t d = 3; d <= x / d; d += 2) {
        if (x % d == 0) {
            sum += d;
            while (x % d == 0) x /= d;
        }
    }
    if (x > 1) sum += x;
    return sum;
}

static bool A082883_isprime(uint64_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d <= n / d; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

static uint64_t A082883_A075860(uint64_t cur) {
    while (!A082883_isprime(cur)) cur = A082883_sumdpf(cur);
    return cur;
}

#define A082883_CAP 65536

static uint64_t A082883_cache[A082883_CAP];
static uint64_t A082883_count = 0;
static uint64_t A082883_p = 2;

seq_t a(idx_t n) {
    if (n < 1 || n > A082883_CAP) return (seq_t)0;
    while (A082883_count < (uint64_t)n) {
        uint64_t p = A082883_p;
        uint64_t q = p + 1;
        while (!A082883_isprime(q)) q++;
        if (q - p > 2) {
            uint64_t v = A082883_A075860(p + 1);
            bool ok = true;
            for (uint64_t j = p + 2; j < q; j++) {
                if (A082883_A075860(j) != v) { ok = false; break; }
            }
            if (ok) A082883_cache[A082883_count++] = p;
        }
        A082883_p = q;
    }
    return (seq_t)A082883_cache[n - 1];
}
