/*
 * A107154  Duplicate of A141373.
 * https://oeis.org/A107154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(1) = 3; for n ≥ 2, a(n) is the (n‑1)-th prime p such that p ≡
 *            19 (mod 24)
 * Range:     i8 n <= 4, i16 n <= 46, i32 n <= 46, i64 n <= 46, i128 n <= 46,
 *            big any n
 * Helpers:   mul_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mul_mod64.h"

/* ---------- Miller–Rabin primality test for 64‑bit integers ---------- */

static inline uint64_t pow_mod64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t res = 1 % mod;
    uint64_t base = a % mod;
    while (e) {
        if (e & 1) res = mul_mod64(res, base, mod);
        base = mul_mod64(base, base, mod);
        e >>= 1;
    }
    return res;
}

static bool is_prime_64(uint64_t n)
{
    if (n < 2) return false;
    /* small divisibility test */
    const uint64_t small_primes[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL,
                                    17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }
    /* Miller-Rabin with deterministic bases for 64‑bit numbers */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }
    const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod64(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool comp = true;
        for (int r = 1; r < s; ++r) {
            x = mul_mod64(x, x, n);
            if (x == n-1) {
                comp = false;
                break;
            }
        }
        if (comp) return false;
    }
    return true;
}

/* ---------- Sequence generation ---------- */

static uint64_t *prime19 = NULL;      /* primes ≡ 19 (mod 24) */
static idx_t  prime19_cap = 0;
static idx_t  prime19_len = 0;
static uint64_t next_candidate = 19;  /* next number to test (≡19 mod 24) */

static void grow_prime_array(idx_t new_cap)
{
    uint64_t *new_ptr = (uint64_t *)realloc(prime19, new_cap * sizeof(uint64_t));
    if (!new_ptr) exit(1); /* memory allocation failed */
    prime19 = new_ptr;
    prime19_cap = new_cap;
}

static void add_prime(uint64_t val)
{
    if (prime19_len == prime19_cap) {
        grow_prime_array(prime19_cap ? prime19_cap * 2 : 16);
    }
    prime19[prime19_len++] = val;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3;   /* the first term */

    idx_t target = n - 2;          /* zero‑based index in prime19 array */

    /* generate primes until we have enough */
    while (prime19_len <= target) {
        if (is_prime_64(next_candidate))
            add_prime(next_candidate);
        next_candidate += 24;
    }

    return (seq_t)prime19[target];
}
