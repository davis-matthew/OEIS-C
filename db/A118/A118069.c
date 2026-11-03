/*
 * A118069  a(n) = smallest composite which is > n! and is coprime to n!.
 * https://oeis.org/A118069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest composite > n! that is coprime to n!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 20 (2026-11-03)
 * Helpers:   gcd, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* util functions from the framework */
#include <stdint.h>
#include <string.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

static inline seq_t seq_one(void) { return (seq_t)1; }

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* static cache of factorials */
    static seq_t *fact = NULL;       /* array of seq_t */
    static size_t  fact_cap = 0;     /* current allocated size */
    static idx_t   fact_max = 0;     /* highest factorial computed */

    /* ensure we have enough space (C and C++ compatible cast) */
    if (fact_cap <= n) {
        size_t new_cap = n + 1;
        seq_t *tmp = mem_resize(seq_t, fact, new_cap);
        fact = tmp;
        fact_cap = new_cap;
    }

    /* first entry: 0! = 1 */
    if (fact_max == 0 && fact_cap > 0)
        fact[0] = seq_one();

    /* compute missing factorials */
    for (idx_t i = fact_max + 1; i <= n; ++i) {
        fact[i] = seq_mul(fact[i - 1], (seq_t)i);
    }
    fact_max = n;

    /* an early check: is n! + 1 composite? */
    seq_t one  = seq_one();
    seq_t cand = fact[n] + (seq_t)1;
    if (!is_prime_big(cand))
        return cand;

    /* otherwise search for the first coprime composite > n! */
    for (idx_t t = 2; ; ++t) {
        seq_t m = fact[n] + (seq_t)t;
        /* m must be coprime with n! → gcd == 1 */
        seq_t g = gcd(fact[n], m);
        if (g != one)
            continue;               /* share a prime factor <= n */

        /* and m must be composite */
        if (!is_prime_big(m))
            return m;
    }
}
