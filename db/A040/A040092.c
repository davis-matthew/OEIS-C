/*
 * A040092  Primes p such that x^3 = 22 has no solution mod p.
 * https://oeis.org/A040092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime p such that p ≡ 1 (mod 3) and 22 is not a
 *            cubic residue modulo p (i.e., 22^{(p−1)/3} ≠ 1 (mod p))
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Integer power modulo: works for any seq_t (built‑in or arbitrary‑precision). */
static seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod)
{
    seq_t result = (seq_t)1 % mod;
    seq_t b = base % mod;
    while (exp) {
        if (exp & 1u) result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1u;
    }
    return result;
}

/* 64‑bit integer square root (floor). Uses 128‑bit multiplication to avoid overflow. */
static uint64_t floor_sqrt_uint64(uint64_t n)
{
    uint64_t r = 0;
    for (int i = 63; i >= 0; --i) {
        uint64_t t = r | ((uint64_t)1 << i);
        if ((unsigned __int128)t * t <= n) r = t;
    }
    return r;
}

/* Storage for the sequence values and for primes used during generation. */
static seq_t  *seq_arr   = NULL;
static size_t  seq_cap   = 0;
static size_t  seq_len   = 0;

static uint64_t *primes  = NULL;
static size_t   primes_cap   = 0;
static size_t   primes_len   = 0;
static uint64_t next_candidate = 5;   /* next odd number to test for primality */

/* Grow a dynamic array of seq_t. */
static void grow_seq_array(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap) ? (*cap) * 2 : 16;
        while (new_cap < need) new_cap <<= 1;
        *arr = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* Grow a dynamic array of uint64_t. */
static void grow_uint64_array(uint64_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap) ? (*cap) * 2 : 16;
        while (new_cap < need) new_cap <<= 1;
        *arr = (uint64_t*)realloc(*arr, new_cap * sizeof(uint64_t));
        *cap = new_cap;
    }
}

/* Ensure that seq_len ≥ n by generating primes until enough sequence terms are found. */
static void ensure_sequence(idx_t n)
{
    if (seq_len >= n) return;

    /* initialise prime list with 2 and 3 on first call */
    if (primes_len == 0) {
        grow_uint64_array(&primes, &primes_cap, 2);
        primes[primes_len++] = 2;
        primes[primes_len++] = 3;
        next_candidate = 5;
    }

    while (seq_len < n) {
        uint64_t cand = next_candidate;
        next_candidate += 2;           /* only odd candidates */

        /* primality test by trial division using stored primes. */
        bool is_prime = true;
        uint64_t sq = floor_sqrt_uint64(cand);
        for (size_t i = 0; i < primes_len && primes[i] <= sq; ++i) {
            if (cand % primes[i] == 0) { is_prime = false; break; }
        }
        if (!is_prime) continue;

        /* append new prime */
        grow_uint64_array(&primes, &primes_cap, primes_len + 1);
        primes[primes_len++] = cand;

        /* test the cubic residue condition: only primes ≡ 1 (mod 3) matter. */
        if (cand % 3 == 1) {
            uint64_t exp = (cand - 1) / 3;
            seq_t val = mod_pow((seq_t)22, exp, (seq_t)cand);
            if (val != (seq_t)1) {
                grow_seq_array(&seq_arr, &seq_cap, seq_len + 1);
                seq_arr[seq_len++] = (seq_t)cand;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for n = 0 – return 0 as a guard */
    ensure_sequence((idx_t)n);
    return seq_arr[(size_t)n - 1];
}
