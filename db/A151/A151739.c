/*
 * A151739  Primes which are the sum of ten consecutive composite numbers.
 * https://oeis.org/A151739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A151739: primes which are the sum of ten consecutive composite numbers */
/* The function computes the n‑th term on demand, storing found terms in a static buffer. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* static state */
static seq_t *found_primes = NULL;
static size_t found_cap   = 0;
static size_t found_cnt   = 0;

static seq_t *composites = NULL;
static size_t comps_cap  = 0;
static size_t comps_cnt  = 0;

static idx_t next_window_start = 0;   /* index of first composite in the current window */
static idx_t global_limit      = 0;    /* highest integer examined for primeness */

/* Ensure at least 'need' terms are available in found_primes. */
static void ensure_terms(idx_t need)
{
    if (need == 0) return;
    if (found_cnt >= need) return;

    /* Allocate composite array if required */
    if (!composites) {
        comps_cap   = 64;
        composites  = (seq_t*)malloc(comps_cap * sizeof(seq_t));
    }

    /* Allocate found_primes array if required */
    if (!found_primes) {
        found_cap   = 64;
        found_primes = (seq_t*)malloc(found_cap * sizeof(seq_t));
    }

    /* Initialise composite list until we have at least one composite (4). */
    if (global_limit == 0) {
        global_limit = 4;
        for (idx_t v = 4; v <= global_limit; ++v) {
            seq_t val = (seq_t)v;
            if (!is_prime_big(val)) {
                if (comps_cnt == comps_cap) {
                    comps_cap *= 2;
                    composites = (seq_t*)realloc(composites, comps_cap * sizeof(seq_t));
                }
                composites[comps_cnt++] = val;
            }
        }
    }

    /* Main loop to generate terms until we have the requested amount. */
    while (found_cnt < need) {
        /* Ensure we have enough composites for the next window. */
        while (comps_cnt <= next_window_start + 9) {
            idx_t old_limit = global_limit;
            idx_t new_limit = global_limit * 2;
            for (idx_t v = old_limit + 1; v <= new_limit; ++v) {
                seq_t val = (seq_t)v;
                if (!is_prime_big(val)) {
                    if (comps_cnt == comps_cap) {
                        comps_cap *= 2;
                        composites = (seq_t*)realloc(composites, comps_cap * sizeof(seq_t));
                    }
                    composites[comps_cnt++] = val;
                }
            }
            global_limit = new_limit;
        }

        /* Compute the sum of the current window of ten composites. */
        seq_t sum = 0;
        for (idx_t k = 0; k < 10; ++k) {
            sum += composites[next_window_start + k];
        }

        /* If the sum is prime, store it. */
        if (is_prime_big(sum)) {
            if (found_cnt == found_cap) {
                found_cap *= 2;
                found_primes = (seq_t*)realloc(found_primes, found_cap * sizeof(seq_t));
            }
            found_primes[found_cnt++] = sum;
        }

        ++next_window_start;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for offset 0 */
    ensure_terms(n);
    return found_primes[n - 1];
}
