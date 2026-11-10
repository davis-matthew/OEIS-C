/*
 * A078154  Primes of the form A004154(k) + 1.
 * https://oeis.org/A078154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search: scan A004154(k) = k! with trailing zeros removed, for k
 *            = 1, 2, ...; a(n) is the n-th value of A004154(k) + 1 that is
 *            prime (Miller-Rabin test)
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Validated: standard, pass, 12 terms through n = 12 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static const unsigned A078154_small_primes[25] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47,
    53, 59, 61, 67, 71, 73, 79, 83, 89, 97
};

/* SHARED_HELPER: modular multiplication over seq_t, overflow-safe for native widths */
static seq_t A078154_mulmod(seq_t x, seq_t y, seq_t m) {
    x %= m;
    y %= m;
    if (x == 0 || y == 0) return 0;
    seq_t p = x * y;
    if (p / x == y) return p % m;
    /* overflowed (native type): fall back to add-and-double */
    seq_t r = 0;
    while (y > 0) {
        if ((y & 1) != 0) {
            seq_t t = m - x;
            if (r >= t) r -= t; else r += x;
        }
        seq_t t2 = m - x;
        if (x >= t2) x -= t2; else x += x;
        y >>= 1;
    }
    return r;
}

static seq_t A078154_powmod(seq_t b, seq_t e, seq_t m) {
    seq_t r = 1;
    b %= m;
    while (e > 0) {
        if ((e & 1) != 0) r = A078154_mulmod(r, b, m);
        e >>= 1;
        if (e > 0) b = A078154_mulmod(b, b, m);
    }
    return r;
}

/* SHARED_HELPER: Miller-Rabin primality test over seq_t (deterministic below 3.3e24) */
static bool A078154_is_prime(seq_t c) {
    if (c < 2) return false;
    for (int i = 0; i < 25; i++) {
        seq_t p = (seq_t)A078154_small_primes[i];
        if (c == p) return true;
        if (c % p == 0) return false;
    }
    if (c < (seq_t)9409) return true;
    seq_t d = c - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }
    seq_t cm1 = c - 1;
    for (int i = 0; i < 12; i++) {
        seq_t base = (seq_t)A078154_small_primes[i];
        seq_t x = A078154_powmod(base, d, c);
        if (x == 1 || x == cm1) continue;
        bool comp = true;
        for (unsigned r = 1; r < s; r++) {
            x = A078154_mulmod(x, x, c);
            if (x == cm1) { comp = false; break; }
            if (x == 1) break;
        }
        if (comp) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    seq_t m = 1;
    idx_t count = 0;
    for (idx_t k = 1; ; k++) {
        if (k > 1) {
            seq_t kk = (seq_t)k;
            seq_t p = m * kk;
            if (p / kk != m) return 0; /* overflow of native seq_t */
            m = p;
            while (m % 10 == 0) m /= 10;
        }
        seq_t c = m + 1;
        if (c < m) return 0; /* overflow */
        if (A078154_is_prime(c)) {
            count++;
            if (count == n) return c;
        }
    }
}
