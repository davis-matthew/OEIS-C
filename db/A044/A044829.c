/*
 * A044829  Positive integers having more base-3 runs of even length than odd.
 * https://oeis.org/A044829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   A044829 – Positive integers having more base‑3 runs of even *
 *            length than odd
 * Range:     u8 n <= 32, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Count runs in base‑3 representation of n.
   Returns true if number of even‑length runs > odd‑length runs. */
static bool has_more_even_runs(seq_t n) {
    int prev_digit = -1;
    int run_len = 0;
    int even_runs = 0, odd_runs = 0;
    seq_t temp = n;

    /* Process digits from least significant to most significant.
       Runs are contiguous; direction does not matter. */
    while (temp > 0) {
        int digit = (int)(temp % (seq_t)3);
        temp /= (seq_t)3;

        if (prev_digit == -1) {          /* first digit */
            prev_digit = digit;
            run_len = 1;
        } else if (digit == prev_digit) {  /* same run */
            run_len++;
        } else {                         /* run ended */
            if (run_len % 2 == 0) even_runs++;
            else odd_runs++;
            prev_digit = digit;
            run_len = 1;
        }
    }

    if (prev_digit != -1) { /* account for last run */
        if (run_len % 2 == 0) even_runs++;
        else odd_runs++;
    }

    return even_runs > odd_runs;
}

/* Cache to store already discovered sequence terms */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;
static idx_t last_checked = 0;

/* Return the n-th term of A044829 (1‑based index). */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;    /* undefined for offset 0 */

    /* Extend cache until we have at least n terms */
    while (cache_size < n) {
        idx_t candidate_idx = last_checked + 1;
        seq_t candidate = (seq_t)candidate_idx;
        last_checked = candidate_idx;

        if (has_more_even_runs(candidate)) {
            /* grow cache if needed */
            if (cache_size >= cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 32 : cache_cap * 2;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) return (seq_t)0; /* allocation failure */
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_size++] = candidate;
        }
    }

    return cache[n - 1];
}
