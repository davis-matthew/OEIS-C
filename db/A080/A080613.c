/*
 * A080613  Minimal sequence such that Omega(a(m))<=Omega(a(n)) for m<n, where
 *          Omega=A001222 (sum of exponents in prime factorization).
 * https://oeis.org/A080613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   omega_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A080613 – minimal sequence with Ω(a(m)) ≤ Ω(a(n)) for m < n
   Formula: a(1)=1; for n>1
            Ω(n) = sum of exponents in prime factorisation (A001222)
            spf(n) = smallest prime factor (A020639)
            a(n)= n*spf(n)^(Ω(a(n-1))-Ω(n))
                   if Ω(n) < Ω(a(n-1))
            a(n)= n
            otherwise.
   The sequence is computed incrementally and memoised to support
   arbitrary call order.  All arithmetic is performed with type seq_t.
   Auxiliary values Ω are held in uint8_t arrays, spf in uint32_t. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/omega_big.h"

/* Forward declare the helper to obtain the smallest prime factor of a
   64‑bit integer if the build does not provide one.  On builds that do
   provide the function, the compiler will choose the correct
   implementation automatically. */
#ifndef _smallest_prime_factor
static uint32_t _smallest_prime_factor(uint64_t n);
#endif

/* Local helpers for spf array and prime storage */
static uint32_t *spf_arr = NULL;
static size_t spf_len = 0;

/* Ensure we have smallest prime factor information up to 'limit' */
static void ensure_spf(size_t limit)
{
    if (limit < spf_len) return;
    size_t new_len = limit + 1;
    uint32_t *new_arr = (uint32_t *)realloc(spf_arr, new_len * sizeof(uint32_t));
    if (!new_arr) abort();
    spf_arr = new_arr;
    for (size_t i = spf_len; i < new_len; ++i) spf_arr[i] = 0;
    spf_arr[1] = 1;

    /* Linear sieve */
    static uint32_t *primes = NULL;
    static size_t primes_cnt = 0;
    static size_t primes_cap = 0;
    if (primes_cap == 0) {
        primes_cap = 1024;
        primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
        if (!primes) abort();
    }
    for (size_t i = 2; i < new_len; ++i) {
        if (spf_arr[i] == 0) {
            spf_arr[i] = (uint32_t)i;
            if (primes_cnt == primes_cap) {
                primes_cap <<= 1;
                primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
                if (!primes) abort();
            }
            primes[primes_cnt++] = (uint32_t)i;
        }
        for (size_t pj = 0; pj < primes_cnt; ++pj) {
            uint32_t p = primes[pj];
            size_t ip = i * p;
            if (p > spf_arr[i] || ip >= new_len) break;
            spf_arr[ip] = p;
        }
    }
    spf_len = new_len;
}

/* Compute Ω(n) using the spf array (valid for n <= spf_len-1) */
static uint8_t omega_small(uint32_t n)
{
    uint8_t cnt = 0;
    while (n > 1) {
        uint32_t p = spf_arr[n];
        do {
            n /= p;
            ++cnt;
        } while (n % p == 0);
    }
    return cnt;
}

/* Power function for small exponents (exponent fits in uint8_t) */
static seq_t pow_uint(seq_t base, uint8_t exp)
{
    seq_t result = 1;
    while (exp--) result *= base;
    return result;
}

/* Memoised sequence storage */
static seq_t *a_arr = NULL;          /* a(n) values, 0‑based index */
static uint8_t *a_omega = NULL;      /* Ω(a(n)) values, 0‑based index */
static size_t a_cap = 0;
static size_t a_len = 0;

/* Ensure that a(n) is computed for all n up to 'n' */
static void ensure_a(idx_t n)
{
    if (n <= a_len) return;
    if (a_cap < n) {
        size_t new_cap = (a_cap == 0) ? 16 : a_cap * 2;
        while (new_cap < n) new_cap <<= 1;
        a_arr = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
        a_omega = (uint8_t *)realloc(a_omega, new_cap * sizeof(uint8_t));
        if (!a_arr || !a_omega) abort();
        a_cap = new_cap;
    }
    /* initialise from the first term */
    if (a_len == 0) {
        a_arr[0] = (seq_t)1;
        a_omega[0] = 0;
        a_len = 1;
    }
    /* build up to n */
    uint8_t curOmega = a_omega[a_len - 1];
    for (idx_t idx = (idx_t)a_len + 1; idx <= n; ++idx) {
        uint32_t i = (uint32_t)idx;
        ensure_spf(i);
        uint8_t omega_i = omega_small(i);
        seq_t val;
        if (omega_i < curOmega) {
            /* multiply by spf(i)^(curOmega-omega_i) */
            uint8_t delta = curOmega - omega_i;
            seq_t mult = pow_uint((seq_t)spf_arr[i], delta);
            val = (seq_t)i * mult;
            /* curOmega stays unchanged */
        } else {
            val = (seq_t)i;
            curOmega = omega_i;
        }
        a_arr[idx - 1] = val;
        a_omega[idx - 1] = curOmega;
    }
    a_len = n;
}

/* Public function as requested */
seq_t a(idx_t n)
{
    if (n < 1) {
        /* Behaviour below the offset is undefined; return 0 here. */
        return (seq_t)0;
    }
    ensure_a(n);
    return a_arr[n - 1];
}
