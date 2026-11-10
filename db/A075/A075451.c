/*
 * A075451  Numbers k such that gcd(A075443(k), A000010(k)) = 1.
 * https://oeis.org/A075451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 with gcd(A075443(k), A000010(k)) = 1; found
 *            by direct search over k
 * Range:     u8 n <= 8, u16 n <= 36, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static idx_t A075443_gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static idx_t A075443_mod_inverse(idx_t a, idx_t mod)
{
    int64_t t = 0, newt = 1;
    int64_t r = (int64_t)mod, newr = (int64_t)a;
    while (newr != 0) {
        int64_t q = r / newr;
        int64_t tmp = t - q * newt;
        t = newt;
        newt = tmp;
        tmp = r - q * newr;
        r = newr;
        newr = tmp;
    }
    if (t < 0) t += (int64_t)mod;
    return (idx_t)t;
}

static seq_t A075443_a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t h = 1; h < n; ++h) {
        if (A075443_gcd_idx(h, n) != 1) continue;
        idx_t inv = A075443_mod_inverse(h, n);
        idx_t diff = (h > inv) ? h - inv : inv - h;
        sum += (seq_t)diff;
    }
    return sum / 2;
}

static seq_t A000010_a(idx_t n)
{
    seq_t result = 1;
    idx_t m = n;
    idx_t p;
    for (p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            m /= p;
            result *= (seq_t)(p - 1);
            while (m % p == 0) {
                m /= p;
                result *= (seq_t)p;
            }
        }
    }
    if (m > 1)
        result *= (seq_t)(m - 1);
    return result;
}

static bool A075451_coprime(seq_t x, seq_t y)
{
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x == 1;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 0;
    while (count < n) {
        ++k;
        if (A075451_coprime(A075443_a(k), A000010_a(k)))
            ++count;
    }
    return (seq_t)k;
}
