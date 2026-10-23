/*
 * A103802  Primes p such that 2p - 9 and 2p + 9 are both primes.
 * https://oeis.org/A103802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   mod_mul64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A103802: primes p such that 2p-9 and 2p+9 are primes */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_mul64.h"

/* ------------------------------------------------------------------ */
/* 64‑bit deterministic Miller–Rabin primality test (works for all */

static inline uint64_t mod_pow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp) {
        if (exp & 1ULL) result = mod_mul64(result, base, mod);
        base = mod_mul64(base, base, mod);
        exp >>= 1ULL;
    }
    return result;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    /* small primes */
    static const uint64_t primes[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,
        23ULL,29ULL,31ULL,37ULL
    };
    for (size_t i = 0; i < sizeof(primes)/sizeof(primes[0]); ++i) {
        uint64_t p = primes[i];
        if (n % p == 0) return n == p;
    }

    /* write n-1 as d·2^s with d odd */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1ULL;
        ++s;
    }

    /* deterministic bases for 64‑bit integers */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) return true;
        uint64_t x = mod_pow64(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool composite = true;
        for (unsigned r = 1; r < s; ++r) {
            x = mod_mul64(x, x, n);
            if (x == n-1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*           storage for the sequence terms (1‑based)                  */
static seq_t *terms = NULL;
static idx_t  terms_size = 0;
static idx_t  terms_alloc = 0;

/* last integer examined so far (for incremental generation)         */
static uint64_t last_checked = 0;

/* ------------------------------------------------------------------ */
/* Ensure that terms[0 .. needed-1] are filled.                       */
static void ensure_terms(idx_t needed)
{
    if (needed <= terms_size) return;

    if (!terms) {
        terms_alloc = 64;
        terms = (seq_t *)malloc((size_t)terms_alloc * sizeof(seq_t));
        if (!terms) exit(EXIT_FAILURE);
    }

    while (terms_size < needed) {
        ++last_checked;                    /* candidate prime */
        if (!is_prime_uint64(last_checked)) continue;
        if (last_checked < 7) continue;    /* skip 2,3,5 */

        uint64_t a = 2ULL * last_checked - 9ULL;
        uint64_t b = 2ULL * last_checked + 9ULL;
        if (a < 2) continue;
        if (!is_prime_uint64(a) || !is_prime_uint64(b)) continue;

        /* store the qualifying prime */
        if (terms_size >= terms_alloc) {
            terms_alloc *= 2U;
            seq_t *tmp = (seq_t *)realloc(terms,
                                          (size_t)terms_alloc * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            terms = tmp;
        }
        terms[terms_size++] = (seq_t)last_checked;
    }
}

/* ------------------------------------------------------------------ */
/* Public API: returns a(n), 1‑based.                                 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    ensure_terms(n);
    return terms[n-1];
}
