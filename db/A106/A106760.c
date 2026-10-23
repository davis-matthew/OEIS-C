/*
 * A106760  Primes with digit sum = 20.
 * https://oeis.org/A106760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 595, u32 n <= 9000, u64 n <= 9000,
 *            u128 n <= 9000, big any n
 * Helpers:   digit_sum_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iterate over consecutive integers, keep those that are
   primes and whose decimal digit sum equals 20.  The nth such prime
   is a(n).  */
/* External linkage: seq_t a(idx_t n) */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/digit_sum_u64.h"

/* ---- 64‑bit Miller–Rabin ------------------------------------------------- */
static inline uint64_t mul_mod_u64(uint64_t a, uint64_t b, uint64_t m) {
    return (uint64_t)((__uint128_t)a * b % m);
}
static inline uint64_t pow_mod_u64(uint64_t a, uint64_t e, uint64_t m) {
    uint64_t r = 1;
    while (e) {
        if (e & 1ULL) r = mul_mod_u64(r, a, m);
        a = mul_mod_u64(a, a, m);
        e >>= 1ULL;
    }
    return r;
}
static inline bool is_prime_u64(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        if (n == small[i]) return true;
        if (n % small[i] == 0) return false;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1ULL) == 0) { d >>= 1ULL; ++s; }
    /* Bases valid for all 64‑bit integers */
    static const uint64_t bases[] = {2, 3, 5, 7, 11, 13};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod_u64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod_u64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ---- Sequence function --------------------------------------------------- */
seq_t a(idx_t n) {
    if (n < 1) return 0;               /* offset 1 */

    static uint64_t *list = NULL;
    static size_t sz = 0, cap = 0;
    static uint64_t next_candidate = 2;  /* next integer to test */

    /* Extend the list until we have at least n terms */
    while (sz < n) {
        for (uint64_t cand = next_candidate; ; ++cand) {
            if (is_prime_u64(cand) && digit_sum_u64(cand) == 20) {
                if (sz == cap) {            /* grow storage */
                    cap = cap ? cap * 2 : 16;
                    uint64_t *tmp = (uint64_t *)realloc(list, cap * sizeof(uint64_t));
                    if (!tmp) return 0;     /* allocation failure */
                    list = tmp;
                }
                list[sz++] = cand;
                if (sz == n) {              /* ready; set next start point */
                    next_candidate = cand + 1;
                    break;
                }
            }
        }
    }
    return (seq_t)list[n - 1];
}
