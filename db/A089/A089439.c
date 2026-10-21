/*
 * A089439  6p+13 and p are primes.
 * https://oeis.org/A089439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1624, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mul_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that p and 6p+13 are also prime.
   a(n) returns the nth such prime (offset 1). */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mul_mod64.h"

/* 64‑bit modular multiplication using a 128‑bit intermediate.  The
   compiler for the test harness supports the unsigned __int128 type. */

/* 64‑bit modular exponentiation (binary exponentiation). */
static inline uint64_t pow_mod64(uint64_t a, uint64_t d, uint64_t mod)
{
    uint64_t res = 1;
    while (d) {
        if (d & 1U)
            res = mul_mod64(res, a, mod);
        a = mul_mod64(a, a, mod);
        d >>= 1U;
    }
    return res;
}

/* Deterministic Miller–Rabin test for 64‑bit integers.  It is correct
   for all 64‑bit values. */
static inline bool isPrime64(uint64_t n)
{
    if (n < 2)
        return false;

    /* Test small primes first */
    static const uint64_t small_primes[] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0)
            return n == p;
    }

    /* Write n‑1 = d · 2^s with d odd */
    uint64_t d = n - 1;
    uint64_t s = 0;
    while ((d & 1U) == 0U) {
        d >>= 1U;
        ++s;
    }

    /* Witnesses that prove primality for 64‑bit integers */
    static const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0)
            continue;
        uint64_t x = pow_mod64(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool composite = true;
        for (uint64_t r = 1U; r < s; ++r) {
            x = mul_mod64(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite)
            return false;
    }
    return true;
}

/* ---- Caching of the discovered primes --------------------------------- */
static seq_t *special = NULL;
static size_t special_len = 0;
static size_t special_cap = 0;
static uint64_t next_candidate = 2;      /* start from the first prime candidate */

static void add_special(seq_t val)
{
    if (special_len == special_cap) {
        size_t new_cap = (special_cap == 0) ? 16U : (special_cap << 1);
        seq_t *tmp = (seq_t *)realloc(special, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                       /* allocation failure */
        special = tmp;
        special_cap = new_cap;
    }
    special[special_len++] = val;
}

/* ---- Sequence function ----------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)                /* safety for offset 1 */
        return (seq_t)0;

    while (special_len < (size_t)n) {
        if (isPrime64(next_candidate)) {
            uint64_t m = 6ULL * next_candidate + 13ULL;
            if (isPrime64(m))
                add_special((seq_t)next_candidate);
        }
        ++next_candidate;
    }
    return special[n - 1];
}
