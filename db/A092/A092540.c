/*
 * A092540  Primes such that their binary representation coincides with first
 *          n terms of A051023 for some n.
 * https://oeis.org/A092540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- build the binary prefixes of A051023 (rule-30 centre
 *            column); a(n) is the n-th prefix value that is prime
 *            (Miller-Rabin test)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define A092540_CAP 1200

static uint8_t *A092540_bits = 0;

static void A092540_init(void)
{
    if (A092540_bits) return;
    size_t w = 2 * (size_t)A092540_CAP + 5;
    size_t c = (size_t)A092540_CAP + 2;
    uint8_t *cur = (uint8_t *)calloc(w, 1);
    A092540_bits = (uint8_t *)calloc((size_t)A092540_CAP + 1, 1);
    cur[c] = 1;
    for (size_t s = 0; s < (size_t)A092540_CAP; ++s) {
        A092540_bits[s] = cur[c];
        size_t lo = c - s - 1, hi = c + s + 1;
        uint8_t prev = cur[lo - 1];
        for (size_t i = lo; i <= hi; ++i) {
            uint8_t center = cur[i];
            uint8_t nx = prev ^ (center | cur[i + 1]);
            prev = center;
            cur[i] = nx;
        }
    }
    free(cur);
}

/* (x + y) mod m, with x, y < m, safe against wraparound */
static seq_t A092540_addmod(seq_t x, seq_t y, seq_t m)
{
    seq_t t = m - y;
    if (x >= t) return x - t;
    return x + y;
}

/* (x * y) mod m, x, y < m; direct when the product does not wrap */
static seq_t A092540_mulmod(seq_t x, seq_t y, seq_t m)
{
    if (x == 0 || y == 0) return (seq_t)0;
    seq_t p = x * y;
    if (p / x == y) return p % m;
    seq_t r = 0;
    while (y > 0) {
        if (y & 1) r = A092540_addmod(r, x, m);
        x = A092540_addmod(x, x, m);
        y >>= 1;
    }
    return r;
}

static seq_t A092540_powmod(seq_t b, seq_t e, seq_t m)
{
    seq_t r = (seq_t)1 % m;
    b = b % m;
    while (e > 0) {
        if (e & 1) r = A092540_mulmod(r, b, m);
        e >>= 1;
        if (e > 0) b = A092540_mulmod(b, b, m);
    }
    return r;
}

static bool A092540_is_prime(seq_t v)
{
    static const unsigned sp[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,
                                  53,59,61,67,71,73,79,83,89,97};
    const int nsp = (int)(sizeof(sp) / sizeof(sp[0]));
    if (v < 2) return false;
    for (int i = 0; i < nsp; ++i) {
        if (v == (seq_t)sp[i]) return true;
        if (v % sp[i] == 0) return false;
    }
    if (v < (seq_t)(97 * 97)) return true;
    seq_t d = v - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    seq_t vm1 = v - 1;
    for (int i = 0; i < nsp; ++i) {
        seq_t x = A092540_powmod((seq_t)sp[i], d, v);
        if (x == 1 || x == vm1) continue;
        bool comp = true;
        for (int r = 1; r < s; ++r) {
            x = A092540_mulmod(x, x, v);
            if (x == vm1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    A092540_init();
    seq_t v = 0;
    idx_t found = 0;
    for (int k = 0; k < A092540_CAP; ++k) {
        seq_t v2 = (seq_t)((v << 1) | (seq_t)A092540_bits[k]);
        if ((v2 >> 1) != v) break;   /* overflow of fixed-width type */
        v = v2;
        if (A092540_bits[k] && v > 1 && A092540_is_prime(v)) {
            ++found;
            if (found == n) return v;
        }
    }
    return (seq_t)0;
}
