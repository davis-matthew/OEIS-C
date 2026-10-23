/*
 * A106118  Primes with maximal digit > 1.
 * https://oeis.org/A106118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   estimate_nth_prime, has_digit_gt1
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute primes whose decimal representation contains a digit > 1
   by sieving up to a bound and filtering.  The function returns the
   n‑th such prime, where a(1) = 2.  It uses static storage so that
   repeated calls are efficient. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/estimate_nth_prime.h"   /* estimate_nth_prime(idx_t) */
#include "../utils/has_digit_gt1.h"

static seq_t *good = NULL;          /* primes that satisfy the digit test */
static idx_t good_count = 0;        /* number of primes stored */
static idx_t sieve_limit = 0;       /* highest value up to which the sieve has run */

/* Rebuild the sieve and filter primes up to `limit`.  All primes
   that contain a digit > 1 are stored in `good`. */
static void recompute(idx_t limit)
{
    /* Allocate composite flag array. */
    uint8_t *isComp = (uint8_t *)calloc((size_t)limit + 1, 1);
    if (!isComp) abort();      /* memory allocation failure */

    /* Eratosthenes sieve */
    for (idx_t p = 2; (p * p) <= limit; ++p) {
        if (!isComp[p]) {
            for (idx_t q = p * p; q <= limit; q += p)
                isComp[q] = 1;
        }
    }

    /* Count qualifying primes */
    idx_t new_count = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!isComp[i] && has_digit_gt1((uint64_t)i))
            ++new_count;
    }

    /* Allocate new array for good primes */
    seq_t *new_good = NULL;
    if (new_count > 0) {
        new_good = (seq_t *)malloc((size_t)new_count * sizeof(seq_t));
        if (!new_good) {
            free(isComp);
            abort();              /* memory allocation failure */
        }
    }

    /* Fill the array */
    idx_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!isComp[i] && has_digit_gt1((uint64_t)i))
            new_good[pos++] = (seq_t)i;   /* cast to seq_t is safe */
    }

    /* Replace old data */
    free(isComp);
    free(good);
    good = new_good;
    good_count = new_count;
    sieve_limit = limit;
}

/* Return the n‑th prime whose decimal representation has a digit > 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1 */

    /* Ensure we have at least n qualifying primes */
    if (good_count < n) {
        /* Estimate an upper bound for the n‑th required prime. */
        idx_t target = estimate_nth_prime(n);
        if (target < 100) target = 100;
        if (target <= sieve_limit) target = sieve_limit + 100;

        /* Grow the sieve until we have enough terms. */
        while (good_count < n) {
            recompute(target);
            if (good_count >= n) break;
            target *= 2;   /* double the bound */
        }
    }

    return good[n - 1];
}
