/*
 * A186814  a(n) = smallest number m such that A002144(n) divides
 *          gcd(A002314(n)^2+1,(A002314(n)+m)^2+1).
 * https://oeis.org/A186814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal positive m such that the nth prime p≡1 (mod 4)
 *            divides gcd(k^2+1,(k+m)^2+1), where k is the minimal square‑root
 *            of −1 mod p
 * Range:     u8 n <= 24, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* STANDARD HEADERS */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* UTILS AVAILABLE IN THE BUILD SYSTEM */
#include <limits.h>
#include <string.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/powmod.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* DECLARATION OF seq_t AND idx_t ARE SUPPLIED BY THE BUILD SYSTEM */

/* -------------------------------------------------------------------- */
/* static storage for primes of the form 4k+1 and their minimal roots   */
/* -------------------------------------------------------------------- */
static uint64_t *prime_list   = NULL;  /* primes ≡1 (mod 4)                     */
static seq_t    *sqrt_cache   = NULL;  /* minimal √(−1) mod p for each prime    */
static size_t   prime_cnt    = 0;       /* number of stored primes                */
static size_t   prime_cap    = 0;       /* capacity of the arrays                 */
static uint64_t next_candidate = 5;     /* next odd ≡1 (mod 4) to test             */

/* -------------------------------------------------------------------- */
/* Tonelli–Shanks implementation for a = −1 (mod p)                     */
/* -------------------------------------------------------------------- */
static seq_t tonelli_shanks_minus_one(seq_t p)
{
    /* Solve x^2 ≡ p-1 (mod p),  p prime, p≡1 (mod4) */
    seq_t a = p - 1;                     /* a ≡ −1 (mod p)                 */
    if (a == 0) return 0;

    /* Factor p-1 = q * 2^s with q odd */
    seq_t q = p - 1;
    int s = 0;
    while ((q & 1) == 0) {
        q >>= 1;
        ++s;
    }

    /* Find a quadratic non‑residue z */
    seq_t z = 2;
    while (powmod(z, (uint64_t)((p - 1) / 2), p) != p - 1)
        ++z;

    seq_t c = powmod(z, (uint64_t)q, p);
    seq_t x = powmod(a, (uint64_t)((q + 1) / 2), p);
    seq_t t = powmod(a, (uint64_t)q, p);
    int m = s;

    while (t != 1) {
        /* Find least i (0 < i < m) with t^(2^i) == 1 */
        seq_t tt = t;
        int i = 0;
        do {
            ++i;
            tt = (tt * tt) % p;
        } while (tt != 1);

        int shift = m - i - 1;
        uint64_t exp = 1ULL << shift;
        seq_t b = powmod(c, exp, p);

        x = (x * b) % p;
        c = (b * b) % p;
        t = (t * c) % p;
        m = i;
    }
    return x;
}

/* -------------------------------------------------------------------- */
/* Ensure that the internal arrays contain at least n primes of form 4k+1 */
/* -------------------------------------------------------------------- */
static void ensure_primes(idx_t n)
{
    if ((size_t)n <= prime_cnt) return;

    /* Increase capacity */
    size_t new_cap = prime_cap ? prime_cap : 64;
    while (new_cap < (size_t)n) new_cap <<= 1;

    /* Resize prime list */
    uint64_t *new_prime_list = (uint64_t *)realloc(prime_list,
                                 new_cap * sizeof(uint64_t));
    if (!new_prime_list) exit(EXIT_FAILURE);
    prime_list = new_prime_list;

    /* Resize sqrt cache using seq_realloc (handles constructions/destructions) */
    sqrt_cache = seq_realloc(sqrt_cache, prime_cap, new_cap);
    if (!sqrt_cache) exit(EXIT_FAILURE);
    prime_cap = new_cap;

    /* Generate missing primes of the form 4k+1 */
    while (prime_cnt < n) {
        if (isPrimeGeneric((seq_t)next_candidate)) {
            prime_list[prime_cnt] = next_candidate;
            seq_t p = (seq_t)next_candidate;
            seq_t root = tonelli_shanks_minus_one(p);
            seq_t p_minus_root = p - root;
            sqrt_cache[prime_cnt] = (root < p_minus_root) ? root : p_minus_root;
            ++prime_cnt;
        }
        next_candidate += 4;   /* next number ≡1 (mod 4) */
    }
}

/* -------------------------------------------------------------------- */
/* Main sequence function                                               */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0, but harmless */
    ensure_primes(n);

    size_t idx = (size_t)(n - 1);

    seq_t p   = (seq_t)prime_list[idx];
    seq_t k   = sqrt_cache[idx];       /* minimal √(−1) mod p */
    seq_t two_k = k * (seq_t)2;

    seq_t ans;
    if (two_k < p)
        ans = p - two_k;
    else
        ans = (seq_t)2 * p - two_k;

    return ans;
}
