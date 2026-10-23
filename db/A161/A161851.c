/*
 * A161851  Squares which are anagrams of prime numbers.
 * https://oeis.org/A161851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 130, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   swap_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Squares which are anagrams of prime numbers.
   Computed by scanning successive squares and checking whether
   any permutation of the digits forms a prime number. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/swap_int.h"

/* deterministic 64‑bit Miller‑Rabin primality test */
static uint64_t mulmod_uint64(uint64_t a, uint64_t b, uint64_t mod) {
    return (unsigned __int128)a * b % mod;
}

static uint64_t powmod_uint64(uint64_t a, uint64_t e, uint64_t mod) {
    uint64_t r = 1;
    while (e) {
        if (e & 1) r = mulmod_uint64(r, a, mod);
        a = mulmod_uint64(a, a, mod);
        e >>= 1;
    }
    return r;
}

static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof small / sizeof *small; ++i) {
        uint64_t p = small[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] = {2,3,5,7,11,13,17};
    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i) {
        uint64_t a = bases[i];
        if (a % n == 0) continue;
        uint64_t x = powmod_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool ok = false;
        for (int r = 1; r < s; ++r) {
            x = mulmod_uint64(x, x, n);
            if (x == n - 1) { ok = true; break; }
        }
        if (!ok) return false;
    }
    return true;
}

/* helper: swap two ints */

/* compute next lexicographic permutation; return false if no more */ 
static bool next_permutation(int *p, int len) {
    int i = len - 2;
    while (i >= 0 && p[i] >= p[i + 1]) --i;
    if (i < 0) return false;
    int j = len - 1;
    while (p[j] <= p[i]) --j;
    swap_int(&p[i], &p[j]);
    for (int l = i + 1, r = len - 1; l < r; ++l, --r) swap_int(&p[l], &p[r]);
    return true;
}

/* check whether a square has a prime anagram */
static bool has_prime_anagram_uint64(uint64_t sq) {
    int digs[20], len = 0;
    uint64_t t = sq;
    if (t == 0) { digs[len++] = 0; }
    while (t > 0) {
        digs[len++] = t % 10;
        t /= 10;
    }
    /* sort ascending */
    for (int i = 1; i < len; ++i) {
        int key = digs[i];
        int j = i - 1;
        while (j >= 0 && digs[j] > key) { digs[j + 1] = digs[j]; --j; }
        digs[j + 1] = key;
    }
    /* iterate permutations */
    do {
        if (digs[0] == 0) continue;            /* skip leading zero */
        uint64_t val = 0;
        for (int i = 0; i < len; ++i) val = val * 10 + digs[i];
        if (is_prime_uint64(val)) return true;
    } while (next_permutation(digs, len));
    return false;
}

/* ------------------------------------------------------------ */

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for 0, but harmless */

    static seq_t *items = NULL;
    static size_t cnt = 0;            /* number of terms stored */
    static size_t capacity = 0;       /* allocated capacity */
    static uint64_t next_root = 4;    /* next candidate square root to test */

    /* Generate terms until we have at least n */
    while (cnt < n) {
        uint64_t sq64 = next_root * next_root;
        if (has_prime_anagram_uint64(sq64)) {
            seq_t val = (seq_t)sq64;
            if (cnt == capacity) {
                size_t newcap = capacity ? capacity * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(items, newcap * sizeof(seq_t));
                if (!tmp) {
                    /* allocation failed; return 0 for safety */
                    return 0;
                }
                items = tmp;
                capacity = newcap;
            }
            items[cnt++] = val;
        }
        ++next_root;
    }

    return items[n - 1];
}
