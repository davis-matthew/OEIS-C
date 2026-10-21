/*
 * A085093  Distinct primes and composite numbers alternately such that the
 *          sum of a pair of successive terms is prime or composite
 *          alternately.
 * https://oeis.org/A085093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A085093: Distinct primes and composite numbers alternately; the sum
   of successive terms alternates between prime and composite, starting
   with a prime sum for a(1)+a(2).  The term a(n) is the smallest unused
   integer that satisfies the parity constraints for the term and the
   sum with its predecessor.  Implementation uses a greedy construction
   with memoisation of previously computed terms and a bit‑array of
   used integers. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"   /* bool isPrimeGeneric(seq_t n); */

static seq_t *seq_cache      = NULL;   /* 1‑based cache of sequence terms   */
static size_t seq_cache_cap  = 0;      /* capacity of seq_cache, counting slots */
static size_t seq_cache_size = 0;      /* highest index computed so far   */

static bool *used            = NULL;   /* used[i]==true if value i already appears in the sequence */
static size_t used_cap       = 0;      /* capacity of used array (values 0 .. used_cap-1)  */

/* ensure used array can hold indices up to n (inclusive) */
static void ensure_used_capacity(size_t n) {
    if (n < used_cap) return;
    size_t old_cap = used_cap;
    size_t new_cap = old_cap ? old_cap << 1 : 16;
    while (new_cap <= n) new_cap <<= 1;
    bool *tmp = (bool *)realloc(used, (new_cap + 1) * sizeof(bool));
    if (!tmp) abort();
    used = tmp;
    for (size_t i = old_cap; i <= new_cap; ++i) used[i] = false;
    used_cap = new_cap + 1;
}

/* ensure seq_cache can hold indices up to n (inclusive) */
static void ensure_seq_capacity(size_t n) {
    if (n < seq_cache_cap) return;
    size_t old_cap = seq_cache_cap;
    size_t new_cap = old_cap ? old_cap << 1 : 16;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_cache, (new_cap + 1) * sizeof(seq_t));
    if (!tmp) abort();
    seq_cache = tmp;
    seq_cache_cap = new_cap + 1;
}

/* Return the n‑th term of the sequence (n ≥ 1).  For n < 1 the function
   returns 0, which satisfies the unused definition. */
seq_t a(idx_t n) {
    if (n < 1) return 0;

    /* compute up to the required index if not already done */
    if (seq_cache_size < (size_t)n) {
        ensure_seq_capacity((size_t)n);

        /* initialise the first term if not yet done */
        if (seq_cache_size == 0) {
            ensure_used_capacity(2);
            used[2] = true;                /* 2 is the first term, a prime */
            seq_cache[1] = (seq_t)2;
            seq_cache_size = 1;
        }

        /* compute subsequent terms up to n */
        for (size_t idx = seq_cache_size + 1; idx <= (size_t)n; ++idx) {
            seq_t prev = seq_cache[idx - 1];
            seq_t cand = 2;                /* start scanning from the smallest positive integer */

            while (true) {
                /* enlarge used table if necessary for current candidate */
                ensure_used_capacity((size_t)cand);

                /* must be unused and not 1 (1 is neither prime nor composite) */
                if (cand == 1 || used[(size_t)cand]) { ++cand; continue; }

                /* primality of the candidate */
                bool cand_prime = isPrimeGeneric(cand);

                /* index parity requirement */
                if ((idx & 1U) != 0) {          /* idx odd → prime required */
                    if (!cand_prime) { ++cand; continue; }
                } else {                       /* idx even → composite required */
                    if (cand_prime) { ++cand; continue; }
                }

                /* sum with previous term */
                seq_t sum = prev + cand;
                bool sum_prime = isPrimeGeneric(sum);

                /* sum alternation: indices even → prime sum, odd → composite sum */
                bool target_sum_prime = ((idx & 1U) == 0);
                if (sum_prime != target_sum_prime) { ++cand; continue; }

                /* found a valid term */
                break;
            }

            seq_cache[idx] = cand;
            used[(size_t)cand] = true;
        }

        seq_cache_size = (size_t)n;
    }

    return seq_cache[(size_t)n];
}
