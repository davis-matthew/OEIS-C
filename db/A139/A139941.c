/*
 * A139941  Primes of the form 19x^2+8xy+19y^2.
 * https://oeis.org/A139941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime numbers of the form 19x^2 + 8xy + 19y^2
 * Range:     u8 n <= 3, u16 n <= 418, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* residues modulo 1380 for which the quadratic form represents a prime */
static const uint16_t residues[] = {19u,79u,91u,199u,319u,379u,451u,511u,
                                    559u,571u,619u,631u,751u,799u,871u,
                                    919u,931u,1111u,1171u,1279u,1339u,1351u};
static const size_t NUM_RESIDUES = sizeof(residues)/sizeof(residues[0]);

/* static state */
static uint8_t *sieve          = NULL;   /* Sieve array: 1 == prime candidate */
static seq_t *rep_primes       = NULL;   /* array of representation primes */
static idx_t  sieve_limit      = 0;      /* current maximum sieved n */
static size_t rep_count        = 0;      /* number of primes in rep_primes */

/* table of allowed residues */
static uint8_t allowed_residuemap[1380] = {0};
static int residues_initialized = 0;
static void init_residue_map(void)
{
    if (residues_initialized) return;
    for (size_t i = 0; i < NUM_RESIDUES; ++i)
        allowed_residuemap[residues[i]] = 1;
    residues_initialized = 1;
}

/* rebuild sieve and rep_primes up to new_limit */
static void rebuild(idx_t new_limit)
{
    /* allocate and initialize sieve */
    uint8_t *s = (uint8_t*)malloc((size_t)(new_limit + 1));
    if (!s) abort();
    memset(s, 1, (size_t)(new_limit + 1));
    s[0] = s[1] = 0;
    for (idx_t i = 2; (idx_t)i * (idx_t)i <= new_limit; ++i) {
        if (s[i]) {
            for (idx_t j = i * i; j <= new_limit; j += i) s[j] = 0;
        }
    }

    /* count representation primes */
    size_t needed = 0;
    for (idx_t i = 2; i <= new_limit; ++i) {
        if (s[i] && allowed_residuemap[(uintptr_t)i % 1380]) needed++;
    }

    /* allocate rep_primes array */
    seq_t *new_rep = (seq_t*)malloc(needed * sizeof(seq_t));
    if (!new_rep) { free(s); abort(); }

    size_t k = 0;
    for (idx_t i = 2; i <= new_limit; ++i) {
        if (s[i] && allowed_residuemap[(uintptr_t)i % 1380])
            new_rep[k++] = (seq_t)i;
    }

    /* Replace old data */
    free(rep_primes);
    rep_primes = new_rep;
    rep_count  = k;

    free(sieve);
    sieve      = s;
    sieve_limit = new_limit;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n < 1, return 0 as placeholder */

    /* initialize residue map once */
    init_residue_map();

    /* ensure we have at least n representable primes */
    while (rep_count < n) {
        idx_t next_limit = sieve_limit ? sieve_limit * 2 : 200000;
        /* avoid overflow and ensure growth */
        if (next_limit <= sieve_limit) next_limit = sieve_limit + 1;
        rebuild(next_limit);
    }

    return rep_primes[n - 1];
}
