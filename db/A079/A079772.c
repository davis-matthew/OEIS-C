/*
 * A079772  Let C(n) be the n-th composite number; then a(n) is the smallest
 *          number > C(n) and not coprime to C(n).
 * https://oeis.org/A079772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(n) + spf[C(n)] where C(n) is the n-th composite (offset
 *            1) and spf[x] is the smallest prime factor of x. The
 *            implementation sieves up to a dynamic upper bound as needed and
 *            records the composite numbers in order to answer calls in
 *            arbitrary order
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   extend_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/extend_array.h"

/* == Internal dynamic arrays == */

static uint64_t *spf = NULL;          /* smallest prime factor table */
static size_t   spf_capacity = 0;     /* allocated size of spf */

static uint64_t *composites = NULL;   /* list of composite numbers */
static size_t   composites_size = 0;   /* how many composites stored */
static size_t   composites_cap  = 0;   /* allocated capacity of composites */

static size_t   sieve_limit = 0;      /* maximum value currently sieved */

/* ------------------------------------------------------------ */
/* Allocate or reallocate a growing array of uint64_t           */

/* ------------------------------------------------------------ */
/* Simple sieve that fills spf[2..limit] and records composites */
/* ------------------------------------------------------------ */
static void recompute_sieve(size_t limit)
{
    if (limit < 2) return;

    /* Allocate spf array if needed */
    if (spf_capacity < limit + 1) {
        uint64_t *tmp = (uint64_t *)realloc(spf, (limit + 1) * sizeof(uint64_t));
        spf = tmp;
        spf_capacity = limit + 1;
    }
    /* Initialise */
    for (size_t i = 0; i <= limit; ++i) spf[i] = 0;

    /* Standard O(n log log n) sieve */
    for (size_t i = 2; i * i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            for (size_t j = i * i; j <= limit; j += i) {
                if (spf[j] == 0) spf[j] = i;
            }
        }
    }
    for (size_t i = 2; i <= limit; ++i)
        if (spf[i] == 0) spf[i] = (uint64_t)i; /* prime numbers */

    /* Build composites list */
    extend_array(&composites, &composites_cap, limit / 2 + 10); /* rough estimate */
    composites_size = 0;
    for (size_t i = 4; i <= limit; ++i) {
        if (spf[i] != i) { /* not prime */
            if (composites_size == composites_cap) {
                /* enlarge */
                extend_array(&composites, &composites_cap, composites_cap * 2);
            }
            composites[composites_size++] = (uint64_t)i;
        }
    }
    sieve_limit = limit;
}

/* ------------------------------------------------------------ */
/* Guess an upper bound for the n-th composite.        */
/* For the constraints of this problem, a linear factor is safe. */
/* ------------------------------------------------------------ */
static size_t estimate_limit(idx_t n_needed)
{
    /* Simple linear estimate: C(n) is less than n*(log n + log log n)
       For n up to 10^5 this is far below n*20. */
    if (n_needed < 10) return 100;
    return (size_t)(n_needed * 20ULL + 100ULL);
}

/* ------------------------------------------------------------ */
/* Make sure we have at least n_needed composites stored. */
/* ------------------------------------------------------------ */
static void ensure_composites(idx_t n_needed)
{
    if ((size_t)n_needed <= composites_size) return;

    size_t new_limit = estimate_limit((idx_t)(composites_size + n_needed));
    if (new_limit <= sieve_limit) return; /* already sufficient */
    if (new_limit < sieve_limit * 2) new_limit = sieve_limit * 2;
    recompute_sieve(new_limit);
}

/* ------------------------------------------------------------ */
/* Main sequence function. */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;              /* undefined for n = 0, but safe */

    ensure_composites(n);
    uint64_t C = composites[(size_t)n - 1];   /* C(n) */
    uint64_t p = spf[C];                     /* smallest prime factor of C */
    return (seq_t)(C + p);
}
