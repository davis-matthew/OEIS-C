/*
 * A167805  Numbers with primitive root -9.
 * https://oeis.org/A167805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer m such that -9 is a primitive
 *            root modulo m (numbers with primitive root -9)
 * Range:     u8 n <= 42, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* No external utilities are required; all functions are implemented
   locally.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Primitive‑root helper routines                                    */
/* ------------------------------------------------------------------ */

/* Compute x^(e) mod m with 64‑bit operands; uses 128‑bit intermediate
   for safety.  */
static uint64_t powmod_uint64(uint64_t x, uint64_t e, uint64_t m)
{
    if (m == 1) return 0;
    uint64_t res = 1 % m;
    x %= m;
    while (e) {
        if (e & 1)
            res = (uint64_t)((__uint128_t)res * x % m);
        x = (uint64_t)((__uint128_t)x * x % m);
        e >>= 1;
    }
    return res;
}

/* Factor n into distinct prime factors.  The arrays `pf` and `exp` are
   assumed to have length at least 32; their sizes are returned.  */
static size_t factor_uint64(uint64_t n, uint64_t *pf, int *exp)
{
    size_t cnt = 0;
    if (n % 2 == 0) {
        int e = 0;
        while (n % 2 == 0) { n /= 2; e++; }
        pf[cnt]   = 2;
        exp[cnt]  = e;
        cnt++;
    }
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) { n /= p; e++; }
            pf[cnt]   = p;
            exp[cnt]  = e;
            cnt++;
        }
    }
    if (n > 1) {
        pf[cnt]   = n;
        exp[cnt]  = 1;
        cnt++;
    }
    return cnt;
}

/* Compute Euler's totient φ(n) using its prime factorisation. */
static uint64_t phi_uint64(uint64_t n)
{
    if (n == 0) return 0;
    uint64_t result = n;
    uint64_t pf[32];
    int exp[32];
    size_t cnt = factor_uint64(n, pf, exp);
    for (size_t i = 0; i < cnt; ++i)
        result -= result / pf[i];
    return result;
}

/* Determine whether the integer g (assumed coprime to n) is a primitive
   root modulo n.  */
static bool is_primitive_root_uint64(uint64_t n, uint64_t g)
{
    uint64_t phi = phi_uint64(n);
    uint64_t order = phi;

    uint64_t pf[32];
    int exp[32];
    size_t cnt = factor_uint64(phi, pf, exp);
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pf[i];
        while (order % p == 0) {
            uint64_t t = powmod_uint64(g, order / p, n);
            if (t == 1)
                order /= p;
            else
                break;
        }
        if (order == 1) break;
    }
    return order == phi;
}

/* Check that n admits primitive roots, i.e. n = 1, 2, 4, p^k, or
   2p^k with p odd prime.  */
static bool has_primitive_root_uint64(uint64_t n)
{
    if (n == 2 || n == 4) return true;

    uint64_t pf[32];
    int exp[32];
    size_t cnt = factor_uint64(n, pf, exp);

    if (cnt == 1) {
        /* Single prime power.  Exclude 2^k with k>1. */
        return pf[0] != 2;
    }
    if (cnt == 2) {
        /* 2p^k must hold with exponent of 2 equal 1. */
        bool has2 = false;
        int exp2  = 0;
        for (size_t i = 0; i < 2; ++i) {
            if (pf[i] == 2) { has2 = true; exp2 = exp[i]; }
        }
        if (!has2) return false;
        if (exp2 != 1) return false;
        return true;
    }
    return false;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                            */
/* ------------------------------------------------------------------ */

/* Static cache for computed terms. */
static seq_t *seq_cache = NULL;
static size_t seq_len   = 0;
static size_t seq_cap   = 0;
static uint64_t last_checked = 1;   /* last candidate examined */

/* Return the nth term of A167805. */
seq_t a(idx_t n)
{
    /* Initialise cache once. */
    if (seq_cap == 0) {
        seq_cap = 128;
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq_cache) exit(1);
    }

    /* Generate terms until we have at least n of them. */
    while (seq_len < n) {
        ++last_checked;
        uint64_t m = last_checked;

        /* Must be coprime with 9 (i.e. not divisible by 3). */
        if (m % 3 == 0) continue;

        /* n must admit primitive roots. */
        if (!has_primitive_root_uint64(m)) continue;

        /* Compute g = -9 mod m.  g = (m - (9 mod m)) mod m. */
        uint64_t r = 9 % m;
        uint64_t g = (m - r) % m;   /* guarantees 0 <= g < m */

        /* g must be a primitive root modulo m. */
        if (!is_primitive_root_uint64(m, g)) continue;

        /* Store term.  Grow cache if needed. */
        if (seq_len == seq_cap) {
            seq_cap *= 2;
            seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
            if (!seq_cache) exit(1);
        }
        seq_cache[seq_len++] = (seq_t)m;
    }

    return seq_cache[n - 1];
}
