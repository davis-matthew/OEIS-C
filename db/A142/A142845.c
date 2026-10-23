/*
 * A142845  Primes congruent to 47 mod 61.
 * https://oeis.org/A142845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime that is congruent to 47 modulo 61
 * Range:     u8 n <= 1, u16 n <= 103, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"   /* declares bool is_prime(idx_t x); */

static seq_t *prog_primes = NULL;   /* list of primes ≡ 47 (mod 61) */
static size_t prog_cnt   = 0;       /* number of primes stored */
static size_t prog_cap   = 0;       /* allocated capacity */
static idx_t  last_limit = 0;       /* highest sieve limit processed */

/* Ensure that prog_primes contains at least n primes. */
static void ensure_primes_up_to(idx_t n)
{
    if (prog_cnt >= (size_t)n)
        return;

    idx_t limit = last_limit ? last_limit * 2 : 1000;

    /* Loop: increase the sieve limit until we have enough primes. */
    while (1) {
        /* Free previous list if any. */
        free(prog_primes);
        prog_primes = NULL;
        prog_cnt   = 0;
        prog_cap   = 0;

        /* Simple estimate of capacity: number of candidates 61k+47\n <= limit */
        size_t est_cap = (size_t)(limit / 61 + 10);
        if (est_cap == 0) est_cap = 64;
        prog_primes = (seq_t*)malloc(est_cap * sizeof(seq_t));
        if (!prog_primes) return;          /* allocation failure, give up */
        prog_cap = est_cap;

        /* Enumerate candidates of the form 61*k + 47 up to the current limit. */
        for (idx_t p = 47; p <= limit; p += 61) {
            if (is_prime(p)) {
                if (prog_cnt >= prog_cap) {
                    /* Grow the array. */
                    size_t new_cap = prog_cap ? prog_cap * 2 : 64;
                    seq_t *tmp = (seq_t*)realloc(prog_primes, new_cap * sizeof(seq_t));
                    if (!tmp) {
                        free(prog_primes);
                        prog_primes = NULL;
                        prog_cnt = prog_cap = 0;
                        return;            /* allocation failure */
                    }
                    prog_primes = tmp;
                    prog_cap = new_cap;
                }
                prog_primes[prog_cnt++] = (seq_t)p;
            }
        }

        last_limit = limit;
        if (prog_cnt >= (size_t)n)
            break;

        /* Increase limit for next iteration. Protect against overflow. */
        if (limit > (idx_t)0x7fffffffffffffffULL) /* > 2^62-1 */
            limit = (idx_t)0x7fffffffffffffffULL;
        else
            limit *= 2;
    }
}

/* Public entry point */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for 0, but guard it */
    ensure_primes_up_to(n);
    return prog_primes[n - 1];
}
