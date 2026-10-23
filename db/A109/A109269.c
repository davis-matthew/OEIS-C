/*
 * A109269  Numbers n such that n^2 < (1/2)*(prevprime(n^2)+nextprime(n^2)).
 * https://oeis.org/A109269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers n such that n^2 < (prevprime(n^2)+nextprime(n^2))/2 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_mul.h"

/* Miller–Rabin primality test for 64‑bit integers */

static inline uint64_t mod_pow(uint64_t a, uint64_t e, uint64_t m) {
    uint64_t r = 1 % m;
    while (e) {
        if (e & 1) r = mod_mul(r, a, m);
        a = mod_mul(a, a, m);
        e >>= 1;
    }
    return r;
}
static inline bool is_prime_64(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof small / sizeof *small; ++i) {
        if (n == small[i]) return true;
        if (n % small[i] == 0) return false;
    }
    /* n-1 = d * 2^s */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i) {
        uint64_t a = bases[i];
        if (a % n == 0) continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool cont = false;
        for (int r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* previous and next prime (inclusive of input > prime) */
static inline uint64_t prev_prime(uint64_t x) {
    if (x <= 2) return 0; /* no prime < 2 */
    uint64_t y = x - 1;
    while (y >= 2 && !is_prime_64(y)) --y;
    return y;
}
static inline uint64_t next_prime(uint64_t x) {
    uint64_t y = x + 1;
    while (!is_prime_64(y)) ++y;
    return y;
}

/* sequence generator */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for OEIS but avoid crash */

    static seq_t *buf = NULL;
    static idx_t buflen = 0;
    static idx_t bufcap = 0;
    static uint64_t candidate = 1;

    while (buflen < n) {
        ++candidate;
        uint64_t sq = (uint64_t)candidate * (uint64_t)candidate;
        uint64_t p = prev_prime(sq);
        uint64_t q = next_prime(sq);
        if ( (unsigned __int128)sq * 2ULL < (unsigned __int128)(p + q) ) {
            if (buflen == bufcap) {
                bufcap = (bufcap == 0) ? 16 : bufcap * 2;
                buf = (seq_t*)realloc(buf, bufcap * sizeof(seq_t));
            }
            buf[buflen++] = (seq_t)candidate;
        }
    }
    return buf[n-1];
}
