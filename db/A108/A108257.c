/*
 * A108257  Numbers k such that concatenating k and the sum of factorials of
 *          the digits of k produces a prime.
 * https://oeis.org/A108257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 769, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9925 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute sequence A108257:
   numbers k such that concatenating k and the sum of factorials of
   the decimal digits of k produces a prime.  The function
   iterates over k and caches results so that any a(n) can be
   retrieved in O(1) after an initial O(n) search. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t pow10(idx_t exp) {
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i) res *= (seq_t)10;
    return res;
}

/* Return the number of decimal digits of x. */
static idx_t digit_len(seq_t x)
{
    idx_t len = 0;
    seq_t t = x;
    while (t > 0) {
        t /= 10;
        len++;
    }
    return len;
}

/* ---- Miller–Rabin helpers for 64‑bit integers ---- */
static uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t m)
{
    return (unsigned __int128)a * b % m;
}

static uint64_t mod_pow(uint64_t a, uint64_t e, uint64_t m)
{
    uint64_t r = 1;
    while (e)
    {
        if (e & 1ULL)
            r = mod_mul(r, a, m);
        a = mod_mul(a, a, m);
        e >>= 1;
    }
    return r;
}

static bool miller_rabin(uint64_t n)
{
    if (n < 2)          return false;
    if (n % 2ULL == 0)  return n == 2;
    if (n % 3ULL == 0)  return n == 3;

    /* write n-1 = d * 2^s */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1ULL) == 0)
    {
        d >>= 1;
        ++s;
    }

    /* deterministic bases for 64‑bit integers */
    const uint64_t bases[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL};
    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0) return true;          /* base divisible by n: pass */

        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;

        bool composite = true;
        for (unsigned r = 1; r < s; ++r)
        {
            x = mod_mul(x, x, n);
            if (x == n - 1)
            {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* ---- trial division by small primes ---- */
static bool is_prime(uint64_t n)
{
    static const uint32_t small_primes[] =
    {
        2U, 3U, 5U, 7U, 11U, 13U, 17U, 19U, 23U, 29U,
        31U, 37U, 41U, 43U, 47U, 53U, 59U, 61U, 67U, 71U
    };

    for (size_t i = 0; i < sizeof(small_primes) / sizeof(small_primes[0]); ++i)
    {
        uint32_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    return miller_rabin(n);
}

/* Miller–Rabin primality test for arbitrary-precision values. */
static bool isprime_big(seq_t n)
{
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    seq_t d = n - 1;
    idx_t r = 0;
    while ((d & 1) == 0) {
        d = d / 2;
        r = r + 1;
    }

    /* Bases sufficient for a probabilistic test. */
    const uint64_t bases[] = {2,3,5,7,11,13,17,19,23,29};
    for (idx_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        seq_t a = (seq_t)bases[i];
        if (a >= n) break;

        /* modular exponentiation x = a^d mod n */
        seq_t x = 1;
        seq_t base = a % n;
        seq_t exp  = d;
        while (exp > 0) {
            if ((exp & 1) != 0) x = (x * base) % n;
            base = (base * base) % n;
            exp  = exp >> 1;
        }

        if (x == 1 || x == n - 1) continue;

        bool cont = false;
        for (idx_t j = 1; j < r; ++j) {
            x = (x * x) % n;
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static const seq_t factor[10] = {
    (seq_t)1,            /* 0! */
    (seq_t)1,            /* 1! */
    (seq_t)2,            /* 2! */
    (seq_t)6,            /* 3! */
    (seq_t)24,           /* 4! */
    (seq_t)120,          /* 5! */
    (seq_t)720,          /* 6! */
    (seq_t)5040,         /* 7! */
    (seq_t)40320,        /* 8! */
    (seq_t)362880        /* 9! */
};

/* helper: prime test that works for both 64‑bit and larger values */
static inline bool is_prime_seq(seq_t x)
{
    return (sizeof(seq_t)<=sizeof(uint64_t))
           ? (bool)is_prime((uint64_t)x)
           : isprime_big(x);
}

/* cache of discovered terms */
static seq_t *terms = NULL;   /* array of k values */
static size_t terms_cap = 0;  /* allocated capacity */
static size_t terms_len = 0;  /* current number of stored terms */
static idx_t last_k = 0;      /* last integer examined */

/* allocate or grow the terms array if needed */
static void ensure_terms_cap(size_t min_cap)
{
    if (terms_cap < min_cap) {
        size_t new_cap = terms_cap ? terms_cap << 1 : 16;
        if (new_cap < min_cap)
            new_cap = min_cap;
        seq_t *tmp = mem_resize(seq_t, terms, new_cap);
        if (!tmp) abort();
        terms = tmp;
        terms_cap = new_cap;
    }
}

/* sum of factorials of decimal digits of k */
static seq_t digit_factorial_sum(idx_t k)
{
    seq_t sum = 0;
    if (k == 0)
        return factor[0];          /* not used, but safe */
    while (k != 0) {
        int d = (int)(k % 10);
        sum += factor[d];
        k /= 10;
    }
    return sum;
}

/* main sequence accessor */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 */

    /* extend cache until we have n terms */
    while (terms_len < n) {
        ++last_k;
        seq_t sf = digit_factorial_sum(last_k);
        size_t len = digit_len(sf);                           /* number of digits of sf */
        seq_t concat = ((seq_t)last_k) * pow10(len) + sf;     /* concatenate */
        if (is_prime_seq(concat)) {
            ensure_terms_cap(terms_len + 1);
            terms[terms_len++] = last_k;
        }
    }
    return terms[n - 1];
}
