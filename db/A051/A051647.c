/*
 * A051647  Primes p such that 210*p + 1 is also prime.
 * https://oeis.org/A051647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 31, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9288 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: primes p such that 210*p + 1 is prime (OEIS A051647). */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"

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

/* Ensure the prime cache is large enough ----------------------------- */
static void grow_prime(size_t *cap, uint64_t **array, size_t needed)
{
    if (*cap >= needed) return;
    size_t newcap = (*cap > 0) ? (*cap << 1) : 16;
    if (newcap < needed) newcap = needed;
    void *tmp = realloc(*array, newcap * sizeof(uint64_t));
    if (!tmp) exit(EXIT_FAILURE);
    *array = (uint64_t *)tmp;
    *cap   = newcap;
}

/* Cache for the sequence values ------------------------------------- */
static seq_t *seq_cache = NULL;
static size_t seq_len   = 0;
static size_t seq_cap   = 0;

/* Cache for primes used in trial division --------------------------- */
static uint64_t *prime_cache = NULL;
static size_t prime_len      = 0;
static size_t prime_cap      = 0;

/* Next integer to test for primality -------------------------------- */
static uint64_t next_candidate = 2;

/* Ensure the sequence cache is large enough -------------------------- */
static void grow_seq(size_t *cap, seq_t **array, size_t needed)
{
    if (*cap >= needed) return;
    size_t newcap = (*cap > 0) ? (*cap << 1) : 16;
    if (newcap < needed) newcap = needed;
    *array = seq_realloc(*array, *cap, newcap);
    *cap   = newcap;
}

/* Test whether n is prime using the primes already in prime_cache ---- */
static bool is_prime_candidate(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_len; ++i) {
        uint64_t p = prime_cache[i];
        if (p > n / p) break;          /* p*p > n   */
        if (n % p == 0) return false;
    }
    return true;
}

/* Sequence entry ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset starts at 1 */

    while (seq_len < (size_t)n) {
        /* Find the next prime number. */
        uint64_t cand = next_candidate;
        while (true) {
            if (is_prime_candidate(cand)) {
                /* Store the new prime in prime_cache. */
                grow_prime(&prime_cap, &prime_cache, prime_len + 1);
                prime_cache[prime_len++] = cand;

                /* Test 210*cand + 1 for primality. */
                seq_t test = (seq_t)cand * (seq_t)210 + (seq_t)1;
                if (is_prime_big(test)) {
                    grow_seq(&seq_cap, &seq_cache, seq_len + 1);
                    seq_cache[seq_len++] = (seq_t)cand;
                    if (seq_len >= (size_t)n) break;
                }
            }
            ++cand;
        }
        next_candidate = cand + 1;
    }
    return seq_cache[n - 1];
}
