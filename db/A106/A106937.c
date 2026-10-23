/*
 * A106937  Primes of the form 2x^2+2xy+9y^2, with x and y nonnegative.
 * https://oeis.org/A106937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime of the form 2x²+2xy+9y² with x,y ≥ 0
 * Range:     u8 n <= 6, u16 n <= 694, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cmp_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* If the form yields duplicate values they are counted only once. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/cmp_u64.h"

/* --- small integer helpers --- */
static inline uint64_t mul_mod_uint64(uint64_t a, uint64_t b, uint64_t m)
{
    return (unsigned __int128)a * b % m;
}

static inline uint64_t pow_mod_uint64(uint64_t a, uint64_t d, uint64_t m)
{
    uint64_t r = 1 % m;
    while (d) {
        if (d & 1ULL) r = mul_mod_uint64(r, a, m);
        a = mul_mod_uint64(a, a, m);
        d >>= 1ULL;
    }
    return r;
}

/* deterministic Miller–Rabin test for 64‑bit integers */
static bool isPrime64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof small_primes / sizeof *small_primes; ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }

    /* write n‑1 = d · 2^s with d odd */
    uint64_t d = n - 1, s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1ULL;
        ++s;
    }

    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL,
        9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod_uint64(a, d, n);
        if (x == 1ULL || x == n-1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod_uint64(x, x, n);
            if (x == n-1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* --- sequence function --- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* undefined offset, return 0 */

    /* initial bound estimate; will be doubled if not enough primes */
    uint64_t bound = (uint64_t)n * 20ULL + 100ULL;
    if (bound < 100ULL) bound = 100ULL;

    while (1) {
        size_t cap = 128, cnt = 0;
        uint64_t *vals = (uint64_t *)malloc(cap * sizeof(uint64_t));
        if (!vals) return (seq_t)0;  /* allocation failure */

        /* generate all values up to bound */
        for (uint64_t y = 0; 9ULL * y * y <= bound; ++y) {
            uint64_t y2 = 9ULL * y * y;
            for (uint64_t x = 0;; ++x) {
                uint64_t val = 2ULL * x * x + 2ULL * x * y + y2;
                if (val > bound) break;
                if (val >= 2ULL && isPrime64(val)) {
                    if (cnt == cap) {
                        cap <<= 1;
                        uint64_t *tmp = (uint64_t *)realloc(vals, cap * sizeof(uint64_t));
                        if (!tmp) { free(vals); return (seq_t)0; }
                        vals = tmp;
                    }
                    vals[cnt++] = val;
                }
            }
        }

        if (cnt > 1) qsort(vals, cnt, sizeof(uint64_t), cmp_u64);

        /* deduplicate */
        size_t uniq_cnt = 0;
        uint64_t *uniq = (uint64_t *)malloc(cnt * sizeof(uint64_t));
        if (!uniq) { free(vals); return (seq_t)0; }

        for (size_t i = 0; i < cnt; ++i) {
            if (i == 0 || vals[i] != vals[i-1]) {
                uniq[uniq_cnt++] = vals[i];
            }
        }
        free(vals);

        if (uniq_cnt >= (size_t)n) {
            uint64_t result = uniq[n-1];
            free(uniq);
            return (seq_t)result;
        }

        free(uniq);
        bound <<= 1;  /* double the search limit and try again */
    }
}
