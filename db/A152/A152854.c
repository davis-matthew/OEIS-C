/*
 * A152854  Numbers n such that either 2n+3 is not prime or 4n+7 is not prime.
 * https://oeis.org/A152854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 231, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers n such that either 2n+3 is not prime or 4n+7 is not prime. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Simple deterministic primality test for 64‑bit integers. */
static inline bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if ((n & 1) == 0) return n == 2;
    if ((n % 3) == 0) return n == 3;

    uint64_t i = 5;
    uint64_t w = 2;
    while (i * i <= n) {
        if (n % i == 0) return false;
        i += w;
        w = 6 - w;
    }
    return true;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t len = 0;
    static size_t cap = 0;
    static idx_t last_checked = 0;

    while (len < n) {
        idx_t cand;
        bool keep;
        do {
            cand = ++last_checked;
            uint64_t v1 = (uint64_t)cand * 2 + 3;
            uint64_t v2 = (uint64_t)cand * 4 + 7;
            bool p1 = is_prime_uint64(v1);
            bool p2 = is_prime_uint64(v2);
            keep = !(p1 && p2);
        } while (!keep);

        if (len == cap) {
            cap = (cap == 0) ? 16 : cap * 2;
            cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        }
        cache[len++] = (seq_t)cand;
    }

    return cache[n - 1];
}
