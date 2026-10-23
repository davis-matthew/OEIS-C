/*
 * A154311  Numbers n such that sum of digits of n > sum of digits of
 *          prime(n).
 * https://oeis.org/A154311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute sequence A154311:  
 * a(n) = the n‑th natural number m such that sum_digits(m) >
 *        sum_digits( prime(m) ).  The function searches over the
 *        natural numbers, generates primes on demand with
 *        is_prime_small, and compares digit sums using digit_sum.
 *        Results are cached so that consecutive calls are fast.
 */

#include <stdlib.h>
#include "../utils/digit_sum.h"
#include "../utils/is_prime_small.h"

static idx_t *primes      = NULL;   /* stored prime values */
static size_t primes_cap  = 0;      /* allocated capacity of primes */
static size_t primes_cnt  = 0;      /* number of primes stored */
static idx_t  last_checked = 1;    /* last integer tested for primality */

/* Ensure that the primes array can hold at least `need` elements. */
static void ensure_primes_cap(size_t need)
{
    if (need <= primes_cap) return;
    size_t new_cap = primes_cap ? primes_cap * 2 : 1;
    while (new_cap < need) new_cap <<= 1;
    idx_t *tmp = (idx_t*)realloc(primes, new_cap * sizeof(idx_t));
    if (!tmp) exit(EXIT_FAILURE);
    primes      = tmp;
    primes_cap  = new_cap;
}

/* Generate primes until we have at least `idx` + 1 primes (0‑based). */
static void ensure_primes_up_to_idx(idx_t idx)
{
    /* idx is zero‑based, so we need primes_cnt > idx */
    while (primes_cnt <= idx) {
        /* Find the next prime after last_checked */
        do {
            ++last_checked;
        } while (!is_prime_small((seq_t)last_checked));
        /* Store it */
        ensure_primes_cap(primes_cnt + 1);
        primes[primes_cnt++] = last_checked;
    }
}

/* Return the n-th term of A154311 (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined key, avoid UB */
    idx_t found    = 0;            /* how many suitable numbers found so far */
    idx_t candidate = 1;           /* natural numbers starting from 1 */

    for (;;) {
        /* Ensure we know the candidate‑th prime: prime_index = candidate-1 */
        ensure_primes_up_to_idx(candidate - 1);

        seq_t sum_candidate = digit_sum((seq_t)candidate);
        seq_t sum_prime     = digit_sum((seq_t)primes[candidate - 1]);

        if (sum_candidate > sum_prime) {
            ++found;
            if (found == n) return candidate;
        }
        ++candidate;
    }
}
