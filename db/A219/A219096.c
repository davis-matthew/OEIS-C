/*
 * A219096  Indices of primes p such that the next two larger primes are p+6
 *          and p+12.
 * https://oeis.org/A219096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = index of the nth prime p such that the next two
 *            consecutive primes are p + 6 and p + 12. The sequence is found
 *            by sieving primes up to a fixed bound, then scanning for
 *            consecutive triples with gaps 6,6
 * Range:     u8 n <= 18, u16 n <= 1652, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mark_composite
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>
#include "../utils/mark_composite.h"

/* ------------------------------------------------------------ */
/*  Parameters  (enough for n up to at least 10 000)              */
/* ------------------------------------------------------------ */
#define SIEVE_LIMIT 50000000U   /* 5·10^7;  ≈ 19 MB of integers, < 4 MB of bitset */

/* ------------------------------------------------------------ */
/*  Static storage                                               */
/* ------------------------------------------------------------ */
static bool initialized = false;
static idx_t *indices = NULL;      /* list of indices of qualifying primes */
static size_t indices_cnt  = 0;    /* how many have been found */
static size_t indices_cap  = 0;    /* allocation capacity for indices */

/* ------------------------------------------------------------ */
/*  Bit‑set helpers for odd numbers only (1‑based index)        */
/* ------------------------------------------------------------ */
static inline bool is_composite(const uint8_t *arr, uint32_t odd_index)
{
    return (arr[odd_index >> 3] & (1U << (odd_index & 7))) != 0;
}

/* ------------------------------------------------------------ */
/*  Build sieve and collect qualifying prime indices            */
/* ------------------------------------------------------------ */
static void build_sieve_and_indices(void)
{
    /* Allocate bit array for odd numbers up to SIEVE_LIMIT */
    size_t bits  = (SIEVE_LIMIT >> 1) + 1;                 /* count of odds ≤ limit */
    size_t bytes = (bits + 7) >> 3;                       /* bytes needed */
    uint8_t *marked = (uint8_t*)calloc(bytes, 1);
    if (!marked) return;                                 /* allocation failed */

    /* Classic odd‑only sieve */
    uint32_t sqrt_lim = (uint32_t) sqrt((double)SIEVE_LIMIT);
    for (uint32_t n = 3; n <= sqrt_lim; n += 2) {
        uint32_t idx = n >> 1;
        if (!is_composite(marked, idx)) {                 /* n is prime */
            for (uint64_t m = (uint64_t)n * n; m <= SIEVE_LIMIT; m += (uint64_t)n + n) {
                uint32_t midx = (uint32_t)(m >> 1);
                mark_composite(marked, midx);
            }
        }
    }

    /* Allocate dynamic array for indices (extendable) */
    indices_cap = 1024;
    indices = (idx_t*)malloc(indices_cap * sizeof(idx_t));
    indices_cnt = 0;

    /* Scan primes, keeping the last three */
    idx_t pos = 1;                     /* prime index starts at 1 (prime 2) */
    uint32_t last_val[3] = {2, 0, 0};   /* values of last primes; last_val[0] oldest */
    idx_t   last_pos[3] = {1, 0, 0};    /* corresponding indices */
    int     last_seen   = 1;            /* how many primes stored (1 after 2) */

    /* Handle prime 2 already counted; iterate over odd numbers */
    for (uint32_t n = 3; n <= SIEVE_LIMIT; n += 2) {
        uint32_t odd_idx = n >> 1;
        if (!is_composite(marked, odd_idx)) {              /* n is prime */
            pos++;                                       /* increment prime index */

            if (last_seen < 3) {                          /* still filling first 3 */
                last_val[last_seen] = n;
                last_pos[last_seen] = pos;
                last_seen++;
            } else {                                      /* have a full triple */
                if (last_val[1] - last_val[0] == 6 &&
                    last_val[2] - last_val[1] == 6) {
                    /* store index of the first prime in the triple */
                    if (indices_cnt == indices_cap) {
                        indices_cap <<= 1;
                        indices = (idx_t*)realloc(indices,
                            indices_cap * sizeof(idx_t));
                    }
                    indices[indices_cnt++] = last_pos[0];
                }
                /* Shift window by one */
                last_val[0] = last_val[1];
                last_val[1] = last_val[2];
                last_val[2] = n;
                last_pos[0] = last_pos[1];
                last_pos[1] = last_pos[2];
                last_pos[2] = pos;
            }
        }
    }

    free(marked);
    initialized = true;
}

/* ------------------------------------------------------------ */
/*  Public interface                                            */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* safety for offset 1 */

    if (!initialized) build_sieve_and_indices();

    if (n > indices_cnt) return (seq_t)0; /* beyond precomputed range */

    return (seq_t)indices[n - 1];
}
