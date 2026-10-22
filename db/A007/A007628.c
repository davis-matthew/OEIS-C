/*
 * A007628  Reflectable emirps.
 * https://oeis.org/A007628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice, easy
 * Formula:   search: enumerate numbers whose digits are all in {0,1,3,8} in
 *            increasing order (base-4 digits mapped 0,1,2,3 -> 0,1,3,8); keep
 *            those that are prime, not palindromes, and whose digit reversal
 *            is also prime
 * Range:     u8 n <= 3, u16 n <= 34, u32 n <= 8600, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static uint64_t mulmod_u64(uint64_t x, uint64_t y, uint64_t m) {
    return (uint64_t)((unsigned __int128)x * y % m);
}

static uint64_t powmod_u64(uint64_t b, uint64_t e, uint64_t m) {
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = mulmod_u64(r, b, m);
        b = mulmod_u64(b, b, m);
        e >>= 1;
    }
    return r;
}

static bool prime_u64(uint64_t n) {
    static const uint64_t small[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    int i;
    uint64_t d;
    int s;
    if (n < 2) return false;
    for (i = 0; i < 12; i++) {
        if (n == small[i]) return true;
        if (n % small[i] == 0) return false;
    }
    d = n - 1;
    s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (i = 0; i < 12; i++) {
        uint64_t x = powmod_u64(small[i], d, n);
        int r;
        if (x == 1 || x == n - 1) continue;
        for (r = 1; r < s; r++) {
            x = mulmod_u64(x, x, n);
            if (x == n - 1) break;
        }
        if (r == s) return false;
    }
    return true;
}

static uint64_t reverse_digits(uint64_t v) {
    uint64_t r = 0;
    while (v > 0) {
        r = r * 10 + v % 10;
        v /= 10;
    }
    return r;
}

static uint64_t *terms = NULL;
static idx_t nterms = 0;
static idx_t capterms = 0;
static uint64_t kstate = 0;

seq_t a(idx_t n) {
    static const uint64_t dmap[4] = {0, 1, 3, 8};
    while (nterms < n) {
        uint64_t k, t, v, p, rv, lead;
        kstate++;
        k = kstate;
        /* last digit must be 1 or 3 */
        if ((k & 3) != 1 && (k & 3) != 2) continue;
        v = 0;
        p = 1;
        t = k;
        lead = 0;
        while (t) {
            lead = dmap[t & 3];
            v += lead * p;
            p *= 10;
            t >>= 2;
        }
        /* leading digit becomes last digit of reversal */
        if (lead != 1 && lead != 3) continue;
        rv = reverse_digits(v);
        if (rv == v) continue;
        if (!prime_u64(v) || !prime_u64(rv)) continue;
        if (nterms == capterms) {
            idx_t nc = capterms ? capterms * 2 : 1024;
            terms = (uint64_t *)realloc(terms, (size_t)nc * sizeof(uint64_t));
            capterms = nc;
        }
        terms[nterms++] = v;
    }
    return (seq_t)terms[n - 1];
}
