/*
 * A088174  Numbers n such that n^2 appears in A088173.
 * https://oeis.org/A088174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sqrt(P(n+1)-P(n)) where P = A088173 (primes 2,3,7,23,...
 *            with increasing square successive differences, greedy); computed
 *            by a search
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-07)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

static bool A088174_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool A088174_is_prime(seq_t n) {
    if (n < 2) return false;
    if (A088174_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);
    for (int i = 0; i < num_bases; i++) {
        seq_t b = (seq_t)base_list[i];
        if (b >= n) continue;
        seq_t x = 1;
        seq_t pw = b % n;
        seq_t e = d;
        while (e > 0) {
            if (e & 1) x = (x * pw) % n;
            pw = (pw * pw) % n;
            e >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    static seq_t *cache = NULL;      /* cache[i] = a(i+1) */
    static size_t size = 0, cap = 0;
    static seq_t prime = 2;          /* current prime of A088173 */
    if (cap == 0) {
        cap = 32;
        cache = mem_new(seq_t, cap);
        if (!cache) return 0;
    }
    while (size < (size_t)n) {
        seq_t s = (size == 0) ? (seq_t)1 : cache[size - 1] + 1;
        for (;; ++s) {
            seq_t cand = prime + s * s;
            if (A088174_is_prime(cand)) {
                prime = cand;
                break;
            }
        }
        if (size == cap) {
            size_t nc = cap * 2;
            seq_t *tmp = mem_resize(seq_t, cache, nc);
            if (!tmp) return 0;
            cache = tmp;
            cap = nc;
        }
        cache[size++] = s;
    }
    return cache[n - 1];
}
