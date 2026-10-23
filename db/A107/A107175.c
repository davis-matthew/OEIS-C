/*
 * A107175  Primes of the form 4x^2 + 17y^2.
 * https://oeis.org/A107175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 404, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Representation: primes of the form 4x^2 + 17y^2 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* ---- fast modular arithmetic for is_prime_uint64 ---- */
static inline uint64_t mod_mul64(uint64_t a, uint64_t b, uint64_t mod) {
    return (unsigned __int128)a * b % mod;
}
static inline uint64_t mod_pow64(uint64_t a, uint64_t e, uint64_t mod) {
    uint64_t r = 1;
    while (e) {
        if (e & 1) r = mod_mul64(r, a, mod);
        a = mod_mul64(a, a, mod);
        e >>= 1;
    }
    return r;
}

/* deterministic Miller–Rabin for 64‑bit integers */
static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] =
        {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = mod_mul64(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (cont) continue;
        return false;
    }
    return true;
}

/* integer sqrt for 64‑bit */
static inline uint64_t int_sqrt(uint64_t x) {
    uint64_t r = (uint64_t) sqrt((long double)x);
    while ((r + 1) * (r + 1) <= x) ++r;
    while (r * r > x) --r;
    return r;
}

/* check if prime p can be written as 4x^2 + 17y^2 */
static bool representable(uint64_t p) {
    for (uint64_t y = 1; ; ++y) {
        unsigned __int128 sq128 = (unsigned __int128)17 * y * y;
        if (sq128 == p) return true;          /* exact match, e.g. p = 17 */
        if (sq128 > p) break;                 /* exceeded, stop searching */
        uint64_t sq = (uint64_t)sq128;        /* now safely fits in uint64_t */
        uint64_t rem = p - sq;
        if (rem & 3u) continue;              /* not divisible by 4 */
        uint64_t x2 = rem >> 2;              /* rem / 4 */
        uint64_t x = int_sqrt(x2);
        if (x * x == x2) return true;        /* found x, y */
    }
    return false;
}

/* memoised generation of the sequence */
static seq_t *seq = NULL;
static idx_t seq_size = 0;
static idx_t seq_cap  = 0;
static uint64_t last_checked = 1;  /* next candidate to test */

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for OEIS; guard condition */
    while (seq_size < n) {
        ++last_checked;
        if (last_checked < 17) continue; /* first relevant prime is 17 */
        if (is_prime_uint64(last_checked) && representable(last_checked)) {
            if (seq_size == seq_cap) {
                seq_cap = seq_cap ? seq_cap << 1 : 16;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_size++] = (seq_t)last_checked;
        }
    }
    return seq[n - 1];
}
