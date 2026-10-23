/*
 * A107819  Slowest increasing sequence where a(n)+n is prime.
 * https://oeis.org/A107819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the smallest integer > a(n−1) with a(1)=1 such that
 *            a(n)+n is prime
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   mul_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mul_mod.h"

/* 64‑bit modular multiplication using 128‑bit intermediate. */

/* 64‑bit modular exponentiation. */
static inline uint64_t modpow(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t res = 1;
    while (e) {
        if (e & 1) res = mul_mod(res, a, mod);
        a = mul_mod(a, a, mod);
        e >>= 1;
    }
    return res;
}

/* Deterministic Miller–Rabin test for 64‑bit integers. */
static inline bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,
                                            19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t a = small_primes[i];
        if (a >= n) break;
        uint64_t x = modpow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (unsigned r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (cont) continue;
        return false;
    }
    return true;
}

/* Sequence function: a(idx_t n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n < 1 */

    /* static memoisation array */
    static seq_t *values = NULL;
    static idx_t capacity = 0;
    static idx_t size = 0;

    if (n <= size) return values[n];

    /* grow the array if necessary */
    if (n + 1 > capacity) {
        idx_t new_cap = capacity ? capacity << 1 : 16;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(values, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure */
        values = tmp;
        capacity = new_cap;
    }

    for (idx_t i = size + 1; i <= n; ++i) {
        if (i == 1) {
            values[1] = (seq_t)1;
            continue;
        }
        seq_t cand = values[i - 1] + 1;
        while (1) {
            /* For n up to about 10000 the sum fits in 64 bits */
            uint64_t test = (uint64_t)(cand + (seq_t)i);
            if (is_prime_uint64(test)) break;
            cand += 1;
        }
        values[i] = cand;
    }
    size = n;
    return values[n];
}
