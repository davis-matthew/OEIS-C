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
 * Range:     u8 n <= 1, u16 n <= 212, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 57 terms through n = 9710 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
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
        seq_t *new_ptr = mem_resize(seq_t, prime_cache, new_cap);
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
