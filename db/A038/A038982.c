/*
 * A038982  Primes p such that 93 is not a square mod p.
 * https://oeis.org/A038982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_mul_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that 93 is a quadratic non‑residue modulo p
   (i.e. the Legendre symbol (93|p) = -1). The function computes the
   n‑th such prime for any call order using a cache. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_mul_uint64.h"

/* --------------------------------------------------------------------
   Modular arithmetic helpers
   -------------------------------------------------------------------- */

/* multiply two unsigned 64‑bit numbers modulo mod, using 128‑bit type */

/* exponentiation modulo mod: base^exp mod mod */
static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp > 0) {
        if (exp & 1)
            result = mod_mul_uint64(result, b, mod);
        b = mod_mul_uint64(b, b, mod);
        exp >>= 1;
    }
    return result;
}

/* --------------------------------------------------------------------
   Deterministic Miller–Rabin primality test for 64‑bit integers
   -------------------------------------------------------------------- */

static bool miller_rabin_pass(uint64_t d, uint64_t n, uint64_t a)
{
    uint64_t x = mod_pow_uint64(a % n, d, n);
    if (x == 1 || x == n - 1)
        return true;
    while (d != n - 1) {
        x = mod_mul_uint64(x, x, n);
        d <<= 1;
        if (x == n - 1)
            return true;
    }
    return false;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;
    /* Check divisibility by small primes first */
    static const uint64_t small_primes[] =
        {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof small_primes / sizeof small_primes[0]; ++i) {
        uint64_t p = small_primes[i];
        if (n == p)
            return true;
        if (n % p == 0)
            return false;
    }
    /* write n-1 = d * 2^s with d odd */
    uint64_t d = n - 1;
    while ((d & 1) == 0)
        d >>= 1;
    /* deterministic bases for 64‑bit integers */
    static const uint64_t test_bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL,
         450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof test_bases / sizeof test_bases[0]; ++i) {
        uint64_t a = test_bases[i];
        if (a % n == 0)
            return true;
        if (!miller_rabin_pass(d, n, a))
            return false;
    }
    return true;
}

/* --------------------------------------------------------------------
   Legendre symbol (93|p) for odd prime p
   -------------------------------------------------------------------- */

static int legendre_symbol_93(uint64_t p)
{
    uint64_t a = 93 % p;
    if (a == 0)
        return 0; /* p divides 93; this does not happen for odd primes >3 */
    uint64_t r = mod_pow_uint64(a, (p - 1) / 2, p);
    if (r == 1)
        return 1;
    if (r == p - 1)
        return -1;
    return 0;
}

/* --------------------------------------------------------------------
   Main sequence function
   -------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* Static cache of computed terms (1‑based index) */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static uint64_t candidate = 3;        /* next odd integer to test */
    static bool initialized = false;

    /* Initialise cache on first call */
    if (!initialized) {
        terms_cap = 1;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms)
            return (seq_t)0;           /* allocation failure – unlikely */
        terms[0] = (seq_t)2;             /* first term of the sequence */
        terms_len = 1;
        initialized = true;
    }

    /* Return cached value if already known */
    if (n > 0 && (size_t)n <= terms_len)
        return terms[(size_t)n - 1];

    /* Generate further terms until reaching desired index */
    while ((size_t)n > terms_len) {
        if (is_prime_uint64(candidate) && legendre_symbol_93(candidate) == -1) {
            /* Ensure capacity */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap * 2;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp)
                    return (seq_t)0;   /* allocation failure */
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)candidate;   /* store the term */
        }
        /* Advance to the next odd candidate */
        candidate += 2;
    }

    return terms[(size_t)n - 1];
}
