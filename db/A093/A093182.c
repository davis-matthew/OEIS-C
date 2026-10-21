/*
 * A093182  Counts where both the odd composites (starting from 1) 1 mod 4 and
 *          3 mod 4 are equal.
 * https://oeis.org/A093182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the count c of odd composites at which the running totals
 *            of odd composites that are 1 mod 4 and 3 mod 4 are equal for the
 *            n-th time (a(1)=5238). The function computes the sequence
 *            lazily: it sieves odd numbers up to a fixed limit (200 000 000)
 *            once, then scans them recording each equality...
 * Range:     u8 none, u16 n <= 12, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Bit‑set helpers for odd numbers only */
#define BIT_INDEX(i)   ((i) >> 3)
#define BIT_OFFSET(i) ((i) & 7)
#define GET_BIT(arr,i) (((arr)[BIT_INDEX(i)] >> BIT_OFFSET(i)) & 1U)
#define SET_BIT(arr,i) ((arr)[BIT_INDEX(i)] |= (1U << BIT_OFFSET(i)))

/* Static data for lazy initialization */
static bool initialized = false;
static const size_t sieve_limit = 200000000;         /* maximum number to sieve */
static unsigned char *sieve_bits = NULL;            /* bit array for odd composites */
static seq_t *cache = NULL;                         /* recorded equal‑count values */
static size_t cache_cap = 0;                        /* capacity of cache */
static size_t cache_len = 0;                        /* number of values stored */

/* Ensure the precomputations are performed only once */
static void precompute(void) {
    if (initialized) return;

    /* Allocate sieve bits array: only odd numbers (including 1) */
    size_t odd_bits = (sieve_limit >> 1) + 1;       /* 1,3,5,… */
    size_t sieve_bytes = (odd_bits + 7) >> 3;
    sieve_bits = (unsigned char *)malloc(sieve_bytes);
    if (!sieve_bits) exit(EXIT_FAILURE);
    for (size_t i = 0; i < sieve_bytes; ++i) sieve_bits[i] = 0;

    /* Classic odd‑only Eratosthenes sieve */
    for (size_t i = 3; i * i <= sieve_limit; i += 2) {
        if (!GET_BIT(sieve_bits, i >> 1)) {          /* i is prime */
            for (size_t j = i * i; j <= sieve_limit; j += i << 1) {
                SET_BIT(sieve_bits, j >> 1);
            }
        }
    }

    /* Prepare cache */
    cache_cap = 20000;   /* sufficient for >10 000 terms (≈19 000 will be found) */
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) exit(EXIT_FAILURE);

    /* Scan odd numbers, recording equal counts */
    seq_t c1 = 0, c3 = 0;  /* counters for 1 mod 4 and 3 mod 4 odd composites */
    for (size_t n = 1; n <= sieve_limit; n += 2) {
        bool composite;
        if (n == 1) {
            composite = true;         /* 1 is treated as odd composite */
        } else {
            composite = GET_BIT(sieve_bits, n >> 1);
        }
        if (composite) {
            if ((n & 3) == 1) c1++;
            else              c3++;
            if (c1 == c3) {
                if (cache_len >= cache_cap) {
                    cache_cap *= 2;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    if (!cache) exit(EXIT_FAILURE);
                }
                cache[cache_len++] = c1;   /* record the common count */
            }
        }
    }
    initialized = true;
}

/* Main sequence routine */
seq_t a(idx_t n) {
    precompute();                                /* ensure data is ready */
    if (n == 0 || (size_t)n > cache_len) return 0; /* out of bounds */
    return cache[(size_t)(n - 1)];
}
