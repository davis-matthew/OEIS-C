/*
 * A070089  P(n) < P(n+1) where P(n) (A006530) is the largest prime factor of
 *          n.
 * https://oeis.org/A070089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th positive integer m such that the largest *
 *            prime divisor of m is strictly smaller than that of m+1, i.e. *
 *            P(m) < P(m+1). The sequence starts
 *            1,2,4,6,8,9,10,12,16,18,20,21,…
 * Range:     u8 n <= 134, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Global state for memoisation */
static size_t seq_len = 0;       /* number of computed terms so far   */
static size_t seq_cap = 0;       /* capacity of seq_vals               */
static seq_t *seq_vals = NULL;   /* array holding the sequence values  */
static idx_t  processed_limit = 0; /* largest integer whose lpf was considered */

/* Ensure seq_vals has at least 'need' slots. */
static void ensure_seq_cap(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = need * 2;
    seq_t *new_buf = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
    if (!new_buf) exit(EXIT_FAILURE);
    seq_vals = new_buf;
    seq_cap  = new_cap;
}

/* Build the sieve up to 'limit' (inclusive) and collect new terms. */
static void extend_sieve(idx_t limit)
{
    /* Allocate smallest prime factor array: indices 0 .. limit+1 */
    idx_t *spf = (idx_t *)calloc((size_t)(limit + 2), sizeof(idx_t));
    if (!spf) exit(EXIT_FAILURE);

    /* Standard sieve to fill spf */
    for (idx_t i = 2; i <= limit + 1; ++i) {
        if (!spf[i]) {
            spf[i] = i;
            /* i might overflow when i*i > limit+1, but unsigned overflow
               is modulo 2^N; use a cautious check */
            for (uint64_t j = (uint64_t)i * i; j <= (uint64_t)limit + 1; j += i) {
                if (!spf[(size_t)j]) spf[(size_t)j] = i;
            }
        }
    }

    /* Allocate array for largest prime factor */
    seq_t *lpf = (seq_t *)malloc((size_t)(limit + 2) * sizeof(seq_t));
    if (!lpf) { free(spf); exit(EXIT_FAILURE); }

    lpf[1] = 1;
    for (idx_t i = 2; i <= limit + 1; ++i) {
        idx_t p = spf[i];
        if (p == i) {                 /* i is prime */
            lpf[i] = (seq_t)p;
        } else {
            seq_t other = lpf[i / p];
            lpf[i] = (other > (seq_t)p) ? other : (seq_t)p;
        }
    }

    /* Collect terms starting just after the previously processed limit. */
    for (idx_t i = processed_limit + 1; i <= limit; ++i) {
        if (lpf[i] < lpf[i + 1]) {
            ensure_seq_cap(seq_len + 1);
            seq_vals[seq_len++] = (seq_t)i;
        }
    }

    /* Clean up */
    free(spf);
    free(lpf);

    processed_limit = limit;
}

/* Extend the sequence until we have at least 'target_n' terms. */
static void compute_up_to(idx_t target_n)
{
    if (seq_len >= (size_t)target_n) return;   /* already enough */

    /* Determine an initial upper bound for the sieve. */
    idx_t limit = (processed_limit == 0)
                  ? (target_n * 3 + 100)
                  : processed_limit * 2 + 100;

    /* Grow the sieve until we accumulate enough terms. */
    while (seq_len < (size_t)target_n) {
        extend_sieve(limit);

        if (seq_len >= (size_t)target_n) break;

        /* Not enough terms yet: increase the limit and try again. */
        limit = limit * 2 + 100;
    }
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* below the OEIS offset */
    compute_up_to(n);
    return seq_vals[n - 1];
}
