/*
 * A027699  Evil primes: primes with even number of 1's in their binary
 *          expansion.
 * https://oeis.org/A027699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 2858, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mem_alloc, mem_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute Evil primes: primes whose binary representation contains an even number of 1's. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem_alloc.h"
#include "../utils/mem_realloc.h"

static inline void grow_arr(seq_t **arr, size_t *cap)
{
    if (*cap == 0) {
        *cap = 8;
        *arr = (seq_t *)mem_alloc(*cap * sizeof(seq_t));
    } else {
        *cap <<= 1; /* double capacity */
        *arr = (seq_t *)mem_realloc(*arr, *cap * sizeof(seq_t));
    }
}

static inline void append_arr(seq_t **arr, size_t *len, size_t *cap, seq_t val)
{
    if (*len >= *cap) grow_arr(arr, cap);
    (*arr)[*len] = val;
    ++(*len);
}

/* ----------------------------------------------------------------------- */
/* check parity of popcount of a 64‑bit integer (true if even) */
static inline bool even_popcount(uint64_t x)
{
    bool parity = false;
    while (x) {
        parity = !parity;
        x &= x - 1;
    }
    return !parity;
}

/* ----------------------------------------------------------------------- */
/* prime test by trial division using previously found primes.  If the
   candidate is prime it is appended to the global prime list. */
static bool test_prime(uint64_t cand, seq_t **primes, size_t *plen, size_t *pcap)
{
    if (cand < 2) return false;

    for (size_t i = 0; i < *plen; ++i) {
        seq_t p = (*primes)[i];
        __uint128_t prod = (__uint128_t)p * (__uint128_t)p;
        if (prod > (__uint128_t)cand) break;
        if ((seq_t)cand % p == 0) return false;
    }

    /* prime found – store it */
    append_arr(primes, plen, pcap, (seq_t)cand);
    return true;
}

/* ----------------------------------------------------------------------- */
/* Evil primes cache ----------------------------------------------------- */
static seq_t *evil_primes   = NULL;
static size_t evil_len     = 0;
static size_t evil_cap     = 0;

/* Prime cache ----------------------------------------------------------- */
static seq_t *primes       = NULL;
static size_t primes_len   = 0;
static size_t primes_cap   = 0;

/* next odd candidate (2 is handled specially) */
static uint64_t next_candidate = 2;

/* ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    size_t target = (size_t)n;                 /* indices are >= 1 */

    /* generate evil primes until we have enough */
    while (evil_len < target) {
        /* search for next prime */
        while (true) {
            if (test_prime(next_candidate, &primes, &primes_len, &primes_cap)) {
                if (even_popcount(next_candidate))
                    append_arr(&evil_primes, &evil_len, &evil_cap, (seq_t)next_candidate);
                break;                       /* processed the prime */
            }
            /* candidate not prime – nothing to do besides advancing */
            break;
        }
        ++next_candidate;                     /* advance to next number */
    }

    return evil_primes[target - 1];
}
