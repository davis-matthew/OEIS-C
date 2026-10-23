/*
 * A106081  Primes p such that 5*p + 12 and 12*p + 5 are both primes.
 * https://oeis.org/A106081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th prime p such that 5p + 12 and 12p + 5 are both
 *            prime
 * Range:     u8 n <= 17, u16 n <= 723, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   add_seq, is_prime32, mod_mul32, mod_pow32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/add_seq.h"
#include "../utils/is_prime32.h"
#include "../utils/mod_mul32.h"
#include "../utils/mod_pow32.h"

/* --------- Global tables and state --------- */
static uint32_t *g_primes = NULL;   /* list of generated primes */
static size_t   g_primes_size = 0;
static seq_t   *g_terms   = NULL;   /* list of sequence terms */
static idx_t     g_terms_size = 0;
static idx_t     g_terms_cap  = 0;
static uint32_t  g_limit = 0;        /* current sieve limit */

/* --------- Build primes and terms up to 'limit' --------- */
static void build_sieve(uint32_t limit)
{
    /* free previous data */
    free(g_primes);
    free(g_terms);
    g_primes = NULL;
    g_terms = NULL;
    g_primes_size = 0;
    g_terms_size = 0;
    g_terms_cap  = 0;

    /* simple sieve of Eratosthenes */
    uint8_t *comp = (uint8_t *)calloc(limit + 1, 1);
    if (!comp) exit(1);
    comp[0] = comp[1] = 1;
    for (uint32_t i = 2; i * i <= limit; ++i) {
        if (!comp[i]) {
            for (uint32_t j = i * i; j <= limit; j += i) comp[j] = 1;
        }
    }

    /* build primes array */
    size_t need = limit / 10 + 10;                /* rough estimate */
    g_primes = (uint32_t *)malloc(need * sizeof(uint32_t));
    if (!g_primes) exit(1);
    for (uint32_t i = 2; i <= limit; ++i) {
        if (!comp[i]) {
            if (g_primes_size == need) {
                need <<= 1;
                g_primes = (uint32_t *)realloc(g_primes, need * sizeof(uint32_t));
                if (!g_primes) exit(1);
            }
            g_primes[g_primes_size++] = i;
        }
    }
    free(comp);
    g_limit = limit;

    /* compute terms */
    for (size_t idx = 0; idx < g_primes_size; ++idx) {
        uint32_t p = g_primes[idx];
        uint32_t x1 = 5u * p + 12;
        uint32_t x2 = 12u * p + 5;
        if (is_prime32(x1) && is_prime32(x2)) {
            add_seq(&g_terms, &g_terms_cap, &g_terms_size, (seq_t)p);
        }
    }
}

/* --------- Main function --------- */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        build_sieve(50000000u);   /* initial limit */
        initialized = true;
    }
    if (n == 0) return (seq_t)0;   /* undefined for n==0, but return 0 */
    if (n <= g_terms_size) return g_terms[n - 1];

    /* need more terms – double the sieve limit until enough */
    uint32_t new_limit = g_limit * 2;
    while (true) {
        build_sieve(new_limit);
        if (n <= g_terms_size) return g_terms[n - 1];
        new_limit *= 2;
    }
}
