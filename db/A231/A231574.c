/*
 * A231574  Primes in A001590.
 * https://oeis.org/A231574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: walk A001590 (a(n) = a(n-1) + a(n-2) + a(n-3), a(0) = 0,
 *            a(1) = 1, a(2) = 0) and keep the terms that pass trial division
 *            plus Miller-Rabin
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 8 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t A231574_addm(seq_t x, seq_t y, seq_t m) {
    seq_t t = m - y;
    if (x >= t) return x - t;
    return x + y;
}

static seq_t A231574_mulm(seq_t x, seq_t y, seq_t m) {
    if (x == 0 || y == 0) return 0;
    seq_t p = x * y;
    if (p / x == y) return p % m;
    seq_t r = 0;
    while (y > 0) {
        if (y % 2 == 1) r = A231574_addm(r, x, m);
        x = A231574_addm(x, x, m);
        y /= 2;
    }
    return r;
}

static seq_t A231574_powm(seq_t b, seq_t e, seq_t m) {
    seq_t r = 1;
    b = b % m;
    while (e > 0) {
        if (e % 2 == 1) r = A231574_mulm(r, b, m);
        e /= 2;
        if (e > 0) b = A231574_mulm(b, b, m);
    }
    return r;
}

static bool A231574_isprime(seq_t m) {
    static const int sp[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,
        67,71,73,79,83,89,97,101,103,107,109,113,127,131,137,139,149,151,157,
        163,167,173,179,181,191,193,197,199};
    const int nsp = (int)(sizeof(sp) / sizeof(sp[0]));
    if (m < 2) return false;
    for (int i = 0; i < nsp; i++) {
        if (m == (seq_t)sp[i]) return true;
        if (m % (seq_t)sp[i] == 0) return false;
    }
    if (m < (seq_t)40000) return true; /* no factor <= 199, and 199^2 > 40000 is false-safe: 211^2=44521 */
    seq_t d = m - 1;
    int s = 0;
    while (d % 2 == 0) { d /= 2; s++; }
    for (int i = 0; i < 13; i++) {
        seq_t x = A231574_powm((seq_t)sp[i], d, m);
        if (x == 1 || x == m - 1) continue;
        bool comp = true;
        for (int r = 1; r < s; r++) {
            x = A231574_mulm(x, x, m);
            if (x == m - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    static seq_t *found = NULL;
    static idx_t cap = 0;
    static idx_t cnt = 0;
    static seq_t x, y, z;
    static bool init = false;
    static bool overflow = false;

    if (!init) {
        cap = 16;
        found = mem_new(seq_t, cap);
        x = 0; y = 1; z = 0;
        init = true;
    }
    if (n == 0) return 0;
    while (cnt < n && !overflow) {
        if (x > 1 && A231574_isprime(x)) {
            if (cnt >= cap) {
                found = mem_resize(seq_t, found, cap * 2);
                cap *= 2;
            }
            found[cnt] = x;
            cnt++;
        }
        seq_t s = x + y;
        if (s < x) { overflow = true; break; }
        seq_t w = s + z;
        if (w < s) { overflow = true; break; }
        x = y;
        y = z;
        z = w;
    }
    if (n <= cnt) return found[n - 1];
    return 0;
}
