/*
 * A142674  Primes congruent to 14 mod 57.
 * https://oeis.org/A142674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 173, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 14 (mod 57) */
/* The function generates the n-th such prime. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *res_primes_ull = NULL; /* array of primes (uint64_t) */
static size_t       res_count     = 0;   /* number of cached primes */
static size_t       res_cap       = 0;   /* allocated capacity */

/* Simple 64‑bit primality test (sufficient for the small numbers in this sequence). */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2U) return false;
    if (n % 2U == 0U) return n == 2U;
    for (uint64_t d = 3U; d * d <= n; d += 2U)
        if (n % d == 0U) return false;
    return true;
}

/* Extend the cache until it contains at least n primes of the form 14+57*k. */
static void generate_residue_primes(idx_t n)
{
    if (res_count >= (size_t)n) return;

    /* initialise storage if needed */
    if (res_cap == 0) {
        res_cap = 128;
        res_primes_ull = (uint64_t *)malloc(res_cap * sizeof(uint64_t));
        if (!res_primes_ull) exit(1);
    }

    /* candidate starts at the smallest prime of the form 14+57*k (71) or after the last stored prime */
    uint64_t candidate;
    if (res_count == 0)
        candidate = 71ULL;          /* first prime ≡14 (mod 57) */
    else
        candidate = res_primes_ull[res_count - 1] + 57ULL;

    while (res_count < (size_t)n) {
        if (is_prime_uint64(candidate)) {
            /* grow array if needed */
            if (res_count >= res_cap) {
                res_cap <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(res_primes_ull,
                                                    res_cap * sizeof(uint64_t));
                if (!tmp) { free(res_primes_ull); exit(1); }
                res_primes_ull = tmp;
            }
            res_primes_ull[res_count++] = candidate;
        }
        candidate += 57ULL;          /* next number in the residue class */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 0 */
    generate_residue_primes(n);
    return (seq_t)res_primes_ull[(size_t)(n - 1)];
}
