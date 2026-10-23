/*
 * A014661  Numbers that do not divide 2^k + 1 for any k>0.
 * https://oeis.org/A014661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 197, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A014661: Numbers that do not divide 2^k + 1 for any k>0.
   Computed by testing the multiplicative order of 2 modulo n and
   verifying that no exponent k satisfies 2^k ≡ –1 (mod n).  Even
   numbers are automatically included. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"

/* ---------- small prime table (up to 1000) ---------- */
static bool primes_initialized = false;
static idx_t primes[200];
static size_t primes_cnt = 0;

static void init_primes(void)
{
    if (primes_initialized)
        return;
    const idx_t LIMIT = 1000;
    bool is_prime[1001];
    for (idx_t i = 0; i <= LIMIT; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (idx_t i = 2; i * i <= LIMIT; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i])
            primes[primes_cnt++] = i;
    primes_initialized = true;
}

/* ---------- factorisation helpers ---------- */

/* fills dist array with distinct prime factors of n (n>1).
   dist gets at most 32 entries. */
static void factor_distinct(idx_t n, idx_t *dist, size_t *cnt)
{
    *cnt = 0;
    init_primes();
    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= n; ++i) {
        idx_t p = primes[i];
        if (n % p == 0) {
            dist[(*cnt)++] = p;
            while (n % p == 0)
                n /= p;
        }
    }
    if (n > 1)
        dist[(*cnt)++] = n;
}

/* ---------- test whether an odd n divides 2^k+1 for some k>0 ---------- */
static bool bad_odd(idx_t n)  /* n must be odd and >=3 */
{
    idx_t prims[32], pcnt;
    factor_distinct(n, prims, &pcnt);

    /* compute φ(n) */
    idx_t phi = n;
    for (size_t i = 0; i < pcnt; ++i) {
        idx_t p = prims[i];
        phi = phi / p * (p - 1);
    }

    /* factor φ(n) to get its prime divisors (distinct ones) */
    idx_t phi_prims[32], pcnt2;
    factor_distinct(phi, phi_prims, &pcnt2);

    /* compute multiplicative order of 2 modulo n */
    idx_t order = phi;
    for (size_t i = 0; i < pcnt2; ++i) {
        idx_t q = phi_prims[i];
        while (order % q == 0) {
            idx_t cand = order / q;
            seq_t r = mod_pow((seq_t)2, cand, (seq_t)n);
            if (r == (seq_t)1)
                order = cand;
            else
                break;
        }
    }

    /* if order is even and 2^(order/2) ≡ –1 (mod n) then n is bad */
    if ((order & 1) == 0) {
        idx_t half = order >> 1;
        seq_t r = mod_pow((seq_t)2, half, (seq_t)n);
        if (r == (seq_t)n - (seq_t)1)
            return true;
    }
    return false;
}

/* ---------- sequence generation with memoisation ---------- */
static seq_t *seq_cache = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;
static idx_t next_candidate = 2;

/* helper to grow cache array */
static void ensure_capacity(size_t need)
{
    if (need <= seq_cap)
        return;
    size_t new_cap = seq_cap == 0 ? 32 : seq_cap * 2;
    while (new_cap < need)
        new_cap *= 2;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* not used, offset is 1 */

    /* generate terms until we have n of them */
    while (seq_len < n) {
        idx_t val = next_candidate;
        bool bad = false;

        if ((val & 1) == 0) {
            /* even numbers are always included */
            bad = false;
        } else if (val >= 3) {
            bad = bad_odd(val);
        }

        if (!bad) {
            ensure_capacity(seq_len + 1);
            seq_cache[seq_len++] = (seq_t)val;
        }
        ++next_candidate;
    }

    return seq_cache[n - 1];
}
