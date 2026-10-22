/*
 * A005939  Pseudoprimes to base 10.
 * https://oeis.org/A005939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) is the n-th composite m with gcd(m,10)=1 and
 *            10^(m-1) == 1 (mod m)
 * Range:     u8 n <= 4, u16 n <= 75, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static uint64_t mulmod_u64(uint64_t x, uint64_t y, uint64_t m) {
    return (uint64_t)(((unsigned __int128)x * y) % m);
}

static uint64_t powmod_u64(uint64_t b, uint64_t e, uint64_t m) {
    uint64_t r = 1 % m;
    b %= m;
    while (e > 0) {
        if (e & 1) r = mulmod_u64(r, b, m);
        b = mulmod_u64(b, b, m);
        e >>= 1;
    }
    return r;
}

static bool mr_isprime(uint64_t n) {
    static const uint64_t bases[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    uint64_t d, x;
    int s, i, k;
    if (n < 2) return false;
    for (i = 0; i < 12; i++) {
        if (n == bases[i]) return true;
        if (n % bases[i] == 0) return false;
    }
    d = n - 1;
    s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (i = 0; i < 12; i++) {
        bool comp = true;
        x = powmod_u64(bases[i], d, n);
        if (x == 1 || x == n - 1) continue;
        for (k = 1; k < s; k++) {
            x = mulmod_u64(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

static uint64_t *terms = NULL;
static idx_t nterms = 0;
static idx_t capterms = 0;
static uint64_t next_cand = 3;

seq_t a(idx_t n) {
    while (nterms < n) {
        uint64_t m = next_cand++;
        if ((m & 1) == 0 || m % 5 == 0) continue;
        if (powmod_u64(10, m - 1, m) != 1) continue;
        if (mr_isprime(m)) continue;
        if (nterms == capterms) {
            idx_t nc = capterms ? capterms * 2 : 1024;
            terms = (uint64_t *)realloc(terms, (size_t)nc * sizeof(uint64_t));
            capterms = nc;
        }
        terms[nterms++] = m;
    }
    return (seq_t)terms[n - 1];
}
