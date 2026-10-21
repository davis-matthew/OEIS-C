/*
 * A085910  a(n) = smallest prime(k) not equal to prime(n) and not included
 *          earlier, such that A001223(k) = A001223(n); i.e., prime(k+1) -
 *          prime(k) = prime(n+1) - prime(n); or 0 is no such prime exists.
 * https://oeis.org/A085910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest unused prime whose prime gap equals that of
 *            prime(n+1), computed by lazy prime generation and caching
 * Range:     u8 n <= 23, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/*  Dynamic array for indices of primes sharing the same gap           */
/* ------------------------------------------------------------------ */
typedef struct {
    idx_t *indices;            /* array of prime indices k-1 (0‑based)   */
    size_t size;               /* number of stored indices               */
    size_t cap;                /* capacity of the array                  */
} GapList;

static GapList *gap_lists = NULL;   /* array of GapList indexed by gap value */
static size_t gap_lists_cap = 0;    /* current capacity of gap_lists array    */

/* Append a prime index to the list for a given gap */
static void append_to_gap_list(idx_t gap, idx_t idx)
{
    if (gap >= gap_lists_cap) {
        size_t new_cap = gap_lists_cap == 0 ? 4 : gap_lists_cap;
        while (new_cap <= gap)
            new_cap <<= 1;
        gap_lists = (GapList *)realloc(gap_lists, new_cap * sizeof(GapList));
        for (size_t i = gap_lists_cap; i < new_cap; ++i) {
            gap_lists[i].indices = NULL;
            gap_lists[i].size    = 0;
            gap_lists[i].cap     = 0;
        }
        gap_lists_cap = new_cap;
    }
    GapList *gl = &gap_lists[gap];
    if (gl->size == gl->cap) {
        size_t new_cap = gl->cap == 0 ? 4 : gl->cap * 2;
        gl->indices = (idx_t *)realloc(gl->indices, new_cap * sizeof(idx_t));
        gl->cap = new_cap;
    }
    gl->indices[gl->size++] = idx;
}

/* ------------------------------------------------------------------ */
/*  Prime generation and storage                                       */
/* ------------------------------------------------------------------ */
static uint64_t *primes = NULL;       /* array of prime values                */
static size_t primes_size = 0;        /* number of primes stored               */
static size_t primes_cap = 0;         /* capacity of primes array              */
static unsigned char *used = NULL;    /* used[p] == 1 if prime[p] already used */

/* Ensure that primes_size > target_index  (i.e. at least target_index+1 primes) */
static void ensure_primes(idx_t target_index)
{
    while (primes_size <= (size_t)target_index) {
        uint64_t candidate;
        if (primes_size == 0) {
            candidate = 2;
        } else if (primes_size == 1) {
            candidate = 3;
        } else {
            candidate = primes[primes_size - 1] + 2;
            for (;;) {
                int is_prime = 1;
                for (size_t i = 0; i < primes_size; ++i) {
                    uint64_t p = primes[i];
                    if (p * p > candidate)
                        break;
                    if (candidate % p == 0) {
                        is_prime = 0;
                        break;
                    }
                }
                if (is_prime)
                    break;
                candidate += 2;
            }
        }

        if (primes_size >= primes_cap) {
            size_t new_cap = primes_cap == 0 ? 8 : primes_cap * 2;
            primes = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
            used  = (unsigned char *)realloc(used,  new_cap * sizeof(unsigned char));
            primes_cap = new_cap;
        }

        primes[primes_size] = candidate;
        used[primes_size]   = 0;

        if (primes_size >= 2) {
            idx_t gap = primes[primes_size - 1] - primes[primes_size - 2];
            idx_t k_index = primes_size - 2; /* index of prime(k) in 0‑based array */
            append_to_gap_list(gap, k_index);
        }
        ++primes_size;
    }
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)0;
    }

    /* Ensure primes up to index n are available */
    ensure_primes(n);

    idx_t prev_idx = n - 1;                /* index of prime(n)   */
    idx_t next_idx = n;                    /* index of prime(n+1) */
    idx_t gap = primes[next_idx] - primes[prev_idx];

    for (;;) {
        if (gap >= gap_lists_cap) {
            /* No primes with this gap have been generated yet */
            ensure_primes(primes_size);    /* generates at least one more prime */
            continue;
        }
        GapList *gl = &gap_lists[gap];
        for (size_t j = 0; j < gl->size; ++j) {
            idx_t cand_idx = gl->indices[j];
            if (!used[cand_idx] && cand_idx != prev_idx) {
                used[cand_idx] = 1;
                return (seq_t)primes[cand_idx];
            }
        }
        /* No unused candidate found so far; generate more primes */
        ensure_primes(primes_size);
    }
}
