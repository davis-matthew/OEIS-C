/*
 * A197862  Prime divisor of n which appears the fewest times previously in
 *          the sequence, with ties to the larger prime.
 * https://oeis.org/A197862
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   factor_u64, seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A197862 – Prime divisor of n which appears the fewest times in the
    sequence (ties broken by the larger prime).  Offset: 2 (a(2)=2).  This
    implementation builds the values on demand, storing them in a static
    array and keeping a table of counts for each prime factor.  It works
    for any seq_t and idx_t supplied by the build system, including
    arbitrary‑precision integer classes in C++17.  */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */
#include "../utils/seq_alloc_array.h"   /* seq_alloc_array(size_t n) */
#include "../utils/seq_free_array.h"    /* seq_free_array(seq_t *p) */

/* Sequence storage ------------------------------------------------------- */

static seq_t *terms = NULL;        /* terms[ n ]  holds a(n) for n>=0  */
static size_t terms_cap = 0;       /* allocated capacity (in terms) */
static size_t terms_last = 0;      /* greatest n for which terms[n] is valid */

/* Prime‑count table ------------------------------------------------------- */

struct pc_entry {          /* prime → appearance count so far */
    uint64_t prime;
    uint32_t count;
};

static struct pc_entry *prime_counts = NULL;
static size_t pc_size = 0;
static size_t pc_cap  = 0;

/* Ensure that 'terms' has space for indices up to 'needed-1'.   */
static void ensure_terms_cap(size_t needed)
{
    if (needed <= terms_cap)
        return;

    /* Allocate a larger array, copy old values, and free the old array. */
    size_t new_cap = (terms_cap ? terms_cap * 2 : 16);
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *new_terms = seq_alloc_array(new_cap);
    if (terms) {
        for (size_t i = 0; i < terms_last; ++i)
            new_terms[i] = terms[i];
        seq_free_array(terms);
    }
    terms = new_terms;
    terms_cap = new_cap;
}

/* Ensure that the prime count table has room for at least one entry. */
static void ensure_pc_cap(void)
{
    if (pc_cap)
        return;
    pc_cap = 64;
    prime_counts = (struct pc_entry *)calloc(pc_cap, sizeof(struct pc_entry));
}

/* Linear search for a prime; returns its index or pc_size if not found. */
static size_t pc_find(uint64_t p)
{
    for (size_t i = 0; i < pc_size; ++i)
        if (prime_counts[i].prime == p)
            return i;
    return pc_size;
}

/* Increment the count for prime 'p', adding a new entry if necessary. */
static void pc_inc(uint64_t p)
{
    size_t idx = pc_find(p);
    if (idx < pc_size) {
        prime_counts[idx].count++;
    } else {
        if (pc_size == pc_cap) {
            pc_cap *= 2;
            prime_counts = (struct pc_entry *)realloc(prime_counts,
                                                     pc_cap * sizeof(struct pc_entry));
        }
        prime_counts[pc_size].prime = p;
        prime_counts[pc_size].count = 1;
        pc_size++;
    }
}

/* Return the current count for prime 'p', or 0 if it hasn't appeared yet. */
static uint32_t pc_get(uint64_t p)
{
    size_t idx = pc_find(p);
    return (idx < pc_size) ? prime_counts[idx].count : 0U;
}

/* Compute a(k) for every k from terms_last+1 up to n (inclusive). */
static void ensure_upto(idx_t n)
{
    if ((idx_t)terms_last >= n)
        return;

    ensure_terms_cap((size_t)n + 1);
    ensure_pc_cap();

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    /* The first valuable term is at index 2, but we initialise from 0
       to keep the array tidy – values before the offset are undefined. */
    for (idx_t k = (idx_t)terms_last + 1; k <= n; ++k) {
        /* Factorise k. */
        factor_u64((uint64_t)k, primes, exps, &cnt);

        /* Choose the prime divisor with the smallest count, ties → larger p. */
        uint64_t bestp = 0;
        uint32_t bestcnt = UINT32_MAX;
        for (size_t i = 0; i < cnt; ++i) {
            uint64_t p = primes[i];
            uint32_t c = pc_get(p);
            if (c < bestcnt || (c == bestcnt && p > bestp)) {
                bestcnt = c;
                bestp   = p;
            }
        }

        terms[(size_t)k] = (seq_t)bestp;
        pc_inc(bestp);
    }

    terms_last = (size_t)n;
}

/* -------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;          /* undefined for n < 2; caller should not request */
    ensure_upto(n);
    return terms[(size_t)n];
}
