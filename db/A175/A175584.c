/*
 * A175584  Prime numbers p with property that sums of neighbor digits of p
 *          are all less than 10.
 * https://oeis.org/A175584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime, neighbour_sum_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes whose sums of adjacent digits are all < 10 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime.h"      /* bool is_prime(idx_t x); */
#include "../utils/neighbour_sum_ok.h"

/* Cache for the sequence */
static seq_t *cache     = NULL;
static size_t cache_len = 0;  /* number of terms stored */
static size_t cache_cap = 0;  /* allocated capacity */

/* Last prime that has been checked, used as starting point for the next search */
static uint64_t last_prime_checked = 0;

/* ------------------------------------------------------------ */
/*  Check whether every adjacent digit pair of n sums to < 10  */

/* ------------------------------------------------------------ */
/*  Resize cache if necessary and append a new term            */
/* ------------------------------------------------------------ */
static void add_term(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap == 0 ? 64 : cache_cap * 2;
        seq_t *tmp = (seq_t *)malloc(sizeof(seq_t) * new_cap);
        if (tmp == NULL) abort();          /* out of memory */
        if (cache != NULL) {
            memcpy(tmp, cache, sizeof(seq_t) * cache_len);
            free(cache);
        }
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* ------------------------------------------------------------ */
/*  Main accessor for the sequence a(n)                        */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* the sequence is 1‑based; 0 is a safe undefined value */

    /* Initialise the cache at the first call */
    if (cache_len == 0) {
        add_term((seq_t)2);
        last_prime_checked = 2;
    }

    /* Fill the cache until we have at least n terms */
    while (cache_len < (size_t)n) {
        /* Search for the next prime greater than last_prime_checked */
        uint64_t cand = last_prime_checked + 1;
        for (;;) {
            /* Skip even numbers except 2 */
            if (cand > 2 && (cand & 1) == 0)
                ++cand;
            else if (is_prime(cand))
                break;
            else
                ++cand;
        }
        last_prime_checked = cand;

        if (neighbour_sum_ok(cand))
            add_term((seq_t)cand);
    }

    return cache[n - 1];
}
