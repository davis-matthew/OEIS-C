/*
 * A075639  a(1) = 1 and then group the prime numbers so that the n-th group
 *          contains n numbers with a composite sum: (1), (2, 7), (3, 5, 13),
 *          (11, 17, 19, 23), (29, 31, 37, 41, 47), (43, 53, 59, 61, 67, 71),
 *          ...
 * https://oeis.org/A075639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A075639 – group primes so that the n‑th group of n primes has a composite sum.
   The sequence is generated on demand: a(1) = 1, a(2) = 2, a(3) = 7, a(4) = 3, … */

/* This implementation keeps a static cache of the terms computed so far.
   For each group it repeatedly takes the smallest unused primes until the group
   length is reached.  If the sum of the group is prime, the last prime is
   replaced by the next unused prime and tested again until the sum becomes
   composite.  The primes used in each group are then appended to the output
   sequence.  The static state ensures that calls to `a(n)` can arrive in any
   order. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../utils/is_prime_small.h"   /* int is_prime_small(uint64_t) */
#include "../utils/is_prime_big.h"     /* bool is_prime_big(seq_t) */

static seq_t *terms          = NULL;   /* 1‑based sequence values */
static size_t terms_cap      = 0;
static size_t terms_len      = 0;       /* highest index computed */

static uint64_t *primes     = NULL;    /* list of primes in order */
static size_t primes_cap    = 0;
static size_t primes_len    = 0;

static bool *used           = NULL;    /* used[i] == true if primes[i] already used */
static size_t used_cap      = 0;

static size_t current_group_size = 2; /* next group size to generate */

/* ------------------------------------------------------------------ */
/* Ensure primes[0..need] and used[0..need] exist and are initialized to false. */
static void
ensure_prime_count(size_t need)
{
    while (primes_len <= need) {
        uint64_t cand = (primes_len == 0) ? 2ULL : primes[primes_len - 1] + 1ULL;
        while (!is_prime_small(cand))
            ++cand;

        if (primes_len == primes_cap) {
            size_t new_cap = primes_cap ? primes_cap * 2 : 64;
            uint64_t *tmp_p = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
            bool      *tmp_u = (bool *)realloc(used,   new_cap * sizeof(bool));
            if (!tmp_p || !tmp_u) return;        /* allocation failure – caller will handle */
            primes   = tmp_p;
            used     = tmp_u;
            primes_cap = new_cap;
            used_cap   = new_cap;
            memset(used + primes_len, 0, (new_cap - primes_len) * sizeof(bool));
        }

        primes[primes_len] = cand;
        used[primes_len]   = false;
        ++primes_len;
    }
}

/* ------------------------------------------------------------------ */
/* Return the index of the first unused prime at or after the given start. */
static size_t
find_next_unused(size_t start)
{
    for (size_t i = start;; ++i) {
        ensure_prime_count(i);
        if (!used[i])
            return i;
    }
}

/* ------------------------------------------------------------------ */
/* Ensure terms array can hold data up to the given index (1‑based). */
static void
ensure_terms_size(size_t needed_index)
{
    if (needed_index + 1 < terms_cap)
        return;

    size_t new_cap = terms_cap ? terms_cap : 128;
    while (new_cap <= needed_index + 1)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) return;                /* allocation failure – caller will handle */
    terms = tmp;
    terms_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Compute sequence values up to index `n` (inclusive). */
static void
compute_to(idx_t n)
{
    if (terms_len >= (size_t)n)
        return;

    /* One‑time initialization */
    if (!terms) {
        terms_cap = 128;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms) return;
        terms[1] = (seq_t)1;
        terms_len = 1;

        primes_cap = 64;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        used   = (bool *)malloc(primes_cap * sizeof(bool));
        if (!primes || !used) return;
        used_cap = primes_cap;
        memset(used, 0, primes_cap * sizeof(bool));
    }

    /* Generate groups until we have at least n terms */
    while ((idx_t)terms_len < n) {
        /* Temporary storage for indices of primes in this group */
        size_t *idxs = (size_t *)malloc(current_group_size * sizeof(size_t));
        if (!idxs) return;

        seq_t sum = 0;

        /* Pick the smallest unused primes */
        for (size_t i = 0; i < current_group_size; ++i) {
            size_t idx = find_next_unused(0);
            used[idx] = true;
            idxs[i] = idx;
            sum += (seq_t)primes[idx];
        }

        /* Replace the last prime until the sum is composite */
        while (is_prime_big(sum)) {
            size_t old_last = idxs[current_group_size - 1];
            used[old_last] = false;               /* free the old last prime */

            size_t new_last = find_next_unused(old_last + 1);
            used[new_last] = true;
            idxs[current_group_size - 1] = new_last;

            sum += (seq_t)primes[new_last] - (seq_t)primes[old_last];
        }

        /* Append the group to the sequence */
        size_t new_terms_len = terms_len + current_group_size;
        ensure_terms_size(new_terms_len);
        for (size_t i = 0; i < current_group_size; ++i) {
            terms[++terms_len] = (seq_t)primes[idxs[i]];
        }

        free(idxs);
        ++current_group_size;    /* next group will be one element larger */
    }
}

/* ------------------------------------------------------------------ */
/* Public API: returns the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, but return 0 for safety */
    compute_to(n);
    return terms[n];
}
