/*
 * A140043  Primes of the form 47x^2+38xy+47y^2.
 * https://oeis.org/A140043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime of the form 47*x² + 38*x*y + 47*y². The
 *            function searches integers in the 15 residue classes modulo 1848
 *            that can be represented by 47x²+38xy+47y² and tests them for
 *            primality using is_prime_big. The results are memoised so calls
 *            can be in any order
 * Range:     u8 n <= 1, u16 n <= 211, u32 n <= 41, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static const unsigned int RESIDUES[] = {
    47, 311, 335, 383, 551, 647, 719, 815,
    839, 983, 1175, 1343, 1391, 1511, 1655
};
#define RES_COUNT (sizeof RESIDUES / sizeof RESIDUES[0])

/* Table of acceptable residues modulo 1848. */
static bool residue_ok[1848];
static bool residue_init = false;

/* Prime cache.  Use dynamic allocation so we can grow it as needed. */
static seq_t *prime_cache = NULL;
static size_t prime_cap  = 0;
static size_t prime_cnt  = 0;
static seq_t last_tested = 0;   /* last integer that was examined */

/* One‑time initialization of the residue table. */
static inline void init_residue_table(void)
{
    if (!residue_init) {
        for (size_t i = 0; i < 1848; ++i) residue_ok[i] = false;
        for (size_t i = 0; i < RES_COUNT; ++i) residue_ok[RESIDUES[i]] = true;
        residue_init = true;
    }
}

/* Grow the cache if more space is required. */
static inline void grow_cache(void)
{
    if (prime_cnt >= prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 8;
        seq_t *new_ptr = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
        if (!new_ptr) exit(1);  /* out of memory */
        prime_cache = new_ptr;
        prime_cap   = new_cap;
    }
}

/* Generate primes until at least n are found. */
static inline void ensure_primes_up_to(idx_t n)
{
    init_residue_table();

    while (prime_cnt < n) {
        /* Next candidate integer. */
        seq_t candidate = last_tested + 1;
        last_tested = candidate;

        /* Filter by residue. */
        unsigned long long mod = (unsigned long long)(candidate % 1848ULL);
        if (!residue_ok[mod]) continue;

        /* Primality test. */
        if (is_prime_big(candidate)) {
            grow_cache();
            prime_cache[prime_cnt++] = candidate;
        }
    }
}

/* ------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; undefined for 0 */
    ensure_primes_up_to(n);
    return prime_cache[n - 1];     /* a(1) is the first prime in the list */
}
