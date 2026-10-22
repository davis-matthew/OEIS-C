/*
 * A007839  Number of polynomials of degree n over GF(2) in which the degrees
 *          of all irreducible factors are distinct.
 * https://oeis.org/A007839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   G.f.: Product_{m>=1} (1 + A001037(m)*x^m), A001037(m) =
 *            (1/m)*Sum_{d|m} mu(m/d)*2^d
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   alloc_zero, release, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/u128_to_seq.h"

static int bits_cache = -1; /* 0 means arbitrary precision */

static int seq_bits(void) {
    if (bits_cache < 0) {
        seq_t x = 1;
        int b = 0;
        while (x != 0 && b < 300) { x += x; b++; }
        bits_cache = (x == 0) ? b : 0;
    }
    return bits_cache;
}

static int mobius(uint64_t n) {
    int count = 0;
    uint64_t p;
    if (n == 1) return 1;
    for (p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            n /= p;
            if (n % p == 0) return 0;
            count++;
        }
    }
    if (n > 1) count++;
    return (count % 2 == 0) ? 1 : -1;
}

/* number of irreducible polynomials of degree m over GF(2), exact whenever it fits */
static seq_t pi_irr(idx_t m) {
    idx_t e = 0, o = m, d;
    while ((o & 1) == 0) { o >>= 1; e++; }
    if (seq_bits() == 0) {
        seq_t pos = 0, neg = 0;
        for (d = 1; d <= m; d++) {
            if (m % d == 0) {
                int mu = mobius(m / d);
                if (mu > 0) pos += (seq_t)1 << (d - e);
                else if (mu < 0) neg += (seq_t)1 << (d - e);
            }
        }
        return (pos - neg) / (seq_t)o;
    } else {
        unsigned __int128 s = 0, inv, ou = (unsigned __int128)o;
        int it;
        for (d = 1; d <= m; d++) {
            if (m % d == 0) {
                int mu = mobius(m / d);
                idx_t k = d - e;
                unsigned __int128 t = (k < 128) ? ((unsigned __int128)1 << k) : 0;
                if (mu > 0) s += t;
                else if (mu < 0) s -= t;
            }
        }
        inv = ou;
        for (it = 0; it < 8; it++) inv = inv * ((unsigned __int128)2 - ou * inv);
        return u128_to_seq(s * inv);
    }
}

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (n + 1 > cache_len) {
        idx_t len = n + 1, m, i;
        seq_t *p = alloc_zero(len);
        p[0] = 1;
        for (m = 1; m < len; m++) {
            seq_t pm = pi_irr(m);
            for (i = len - 1; i >= m; i--)
                p[i] += pm * p[i - m];
        }
        if (cache) release(cache, cache_len);
        cache = p;
        cache_len = len;
    }
    return cache[n];
}
