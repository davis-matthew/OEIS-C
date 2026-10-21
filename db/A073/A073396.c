/*
 * A073396  The number n equals the product of two numbers: sums of prime
 *          factors of n, with and without repetition.
 * https://oeis.org/A073396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, full, fini, bref
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A073396: numbers n such that
   n = (sum of distinct prime factors of n) * (sum of prime factors of n with repetition).
   The function returns the n-th term of this sequence (OEIS offset 1). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
/* Utility for dynamic resizing of seq_t arrays */
#include "../utils/seq_realloc.h"

/* Static state shared among calls */
static idx_t   _checked_limit = 0;   /* maximum n that has been scanned so far      */
static uint64_t *spf = NULL;        /* smallest prime factor array                 */
static size_t   spf_cap  = 0;        /* capacity of spf (number of entries)        */

static seq_t *solutions = NULL;      /* array of found solutions for this sequence */
static size_t sol_len  = 0;          /* number of solutions stored                */
static size_t sol_cap  = 0;          /* capacity of solutions array               */

/* Extend the spf array to cover [0..new_limit] (inclusive). */
static void _sieve_spf(idx_t new_limit)
{
    if (new_limit <= (idx_t)spf_cap)
        return;                     /* already covers */

    /* Allocate / reallocate */
    uint64_t *new_spf = (uint64_t *)realloc(spf, (size_t)(new_limit + 1) * sizeof(uint64_t));
    if (!new_spf)
        exit(1);                    /* allocation failed */
    spf = new_spf;
    spf_cap = new_limit;

    /* Initialize spf[i] = i */
    for (idx_t i = 0; i <= new_limit; ++i)
        spf[i] = (uint64_t)i;

    /* Sieve */
    for (idx_t p = 2; p <= new_limit / p; ++p) {
        if (spf[p] == p) {          /* p is prime */
            for (idx_t j = p * p; j <= new_limit; j += p) {
                if (spf[j] == j)
                    spf[j] = p;
            }
        }
    }
}

/* Compute sum of distinct primes and sum with multiplicity for n (≤ spf_cap). */
static void _sums_from_spf(uint64_t n, seq_t *sum_distinct, seq_t *sum_mult)
{
    seq_t s_dist = 0;
    seq_t s_mult = 0;
    uint64_t last_p = 0;

    while (n > 1) {
        uint64_t p = spf[n];
        s_mult += (seq_t)p;
        if (p != last_p) {
            s_dist += (seq_t)p;
            last_p = p;
        }
        n /= p;
    }
    *sum_distinct = s_dist;
    *sum_mult = s_mult;
}

/* Ensure that at least ‘target’ solution terms have been found.
   Expand search by incrementally increasing the upper bound. */
static void _ensure_solutions(idx_t target)
{
    while (sol_len < target) {
        /* Determine new upper bound for scanning */
        idx_t next_limit = _checked_limit + 1000;
        if (next_limit < _checked_limit)      /* overflow guard */
            next_limit = (idx_t)(-1);

        if (next_limit < target * 10)
            next_limit = target * 10;

        _sieve_spf(next_limit);

        for (idx_t n = _checked_limit + 1; n <= next_limit; ++n) {
            seq_t sum_dist, sum_mult;
            _sums_from_spf((uint64_t)n, &sum_dist, &sum_mult);
            seq_t prod = sum_dist * sum_mult;
            if (prod == (seq_t)n) { /* n satisfies the condition */
                if (sol_len == sol_cap) {
                    size_t new_cap = sol_cap == 0 ? 16 : sol_cap * 2;
                    seq_t *tmp = seq_realloc(solutions, sol_cap, new_cap);
                    if (tmp == NULL)
                        exit(1);
                    solutions = tmp;
                    sol_cap = new_cap;
                }
                solutions[sol_len++] = (seq_t)n;
            }
        }
        _checked_limit = next_limit;
    }
}

/* Return the 'n'-th term of the sequence (OEIS offset 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n=0, guard against accidental call */

    _ensure_solutions(n);
    return solutions[n - 1];
}
