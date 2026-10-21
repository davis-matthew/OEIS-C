/*
 * A063446  Integers m such that d(m+1) = 2*d(m), where d(m) = A000005(m).
 * https://oeis.org/A063446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   integers m such that d(m+1) = 2*d(m) (d = divisor count)
 * Range:     u8 n <= 26, u16 n <= 6185, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation keeps a list of all terms found so far and
   recomputes the divisor‑count table in blocks that double in size.
   The algorithm is O(N log N) for the whole table rebuild, which
   suffices for N ≈ 2^18 (the 10 000th term of the sequence). */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

static idx_t current_limit = 0;      /* highest m that has been checked */
static idx_t next_to_check = 1;      /* next m to test after current_limit */
static int initialized = 0;          /* has the sequence been initialized? */

static uint32_t *tau = NULL;         /* divisor counts for 1 .. current_limit+1 */
static size_t tau_size = 0;

/* storage for the found terms (the m values) */
static seq_t *stored_terms = NULL;
static size_t stored_terms_len = 0;
static size_t stored_terms_cap = 0;

/* ------------------------------------------------------------
   Build divisor-count array for numbers 1 .. N+1 (inclusive).
   ------------------------------------------------------------ */
static void compute_divisor_counts(idx_t N)
{
    /* indices 0 .. N+1 inclusive -> N+2 elements */
    size_t new_size = (size_t)N + 2;
    uint32_t *new_tau = (uint32_t *)calloc(new_size, sizeof(uint32_t));
    if (!new_tau) abort();            /* allocation failure          */

    /* simple divisor sieve */
    for (idx_t i = 1; i <= N + 1; ++i) {
        for (idx_t j = i; j <= N + 1; j += i) {
            ++new_tau[(size_t)j];
        }
    }

    /* replace old table */
    free(tau);
    tau = new_tau;
    tau_size = new_size;
}

/* ------------------------------------------------------------
   Ensure the terms array can hold at least "needed" entries.
   ------------------------------------------------------------ */
static void ensure_terms_capacity(size_t needed)
{
    if (needed <= stored_terms_cap) return;
    size_t new_cap = stored_terms_cap ? stored_terms_cap * 2 : 8;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(stored_terms, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    stored_terms = tmp;
    stored_terms_cap = new_cap;
}

/* ------------------------------------------------------------
   Build the table up to "n" terms.  The table is expanded
   in blocks that double in size each time.  The function
   works correctly even if called with arbitrary values of n
   (in any order).
   ------------------------------------------------------------ */
static void ensure_n_terms(idx_t n)
{
    if (!initialized) {
        /* first sweep */
        current_limit = 200000;    /* arbitrary start; good for the first 10 000 terms */
        compute_divisor_counts(current_limit);

        for (idx_t m = 1; m < current_limit; ++m) {
            if (tau[(size_t)(m + 1)] == 2u * tau[(size_t)m]) {
                ensure_terms_capacity(stored_terms_len + 1);
                stored_terms[stored_terms_len++] = (seq_t)m;
            }
        }
        next_to_check = current_limit;
        initialized = 1;
    }

    while (stored_terms_len < (size_t)n) {
        idx_t new_limit = current_limit * 2;
        compute_divisor_counts(new_limit);

        for (idx_t m = next_to_check; m < new_limit; ++m) {
            if (tau[(size_t)(m + 1)] == 2u * tau[(size_t)m]) {
                ensure_terms_capacity(stored_terms_len + 1);
                stored_terms[stored_terms_len++] = (seq_t)m;
            }
        }

        next_to_check = new_limit;
        current_limit = new_limit;
    }
}

/* ------------------------------------------------------------
   Public function required by the grading system.
   Returns the n-th term (1‑based indexing).
   ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* undefined; sequence starts at n = 1 */
    ensure_n_terms(n);
    return stored_terms[(size_t)(n - 1)];
}
