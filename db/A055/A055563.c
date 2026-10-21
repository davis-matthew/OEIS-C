/*
 * A055563  a(n) = least number greater than a(n-1) not the sum of an earlier
 *          pair of consecutive terms, a(0) = 3.
 * https://oeis.org/A055563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least integer > a(n-1) that is not the sum of an *
 *            earlier consecutive pair of terms, with a(0) = 3. * Uses a
 *            simple incremental sieve-like generation
 * Range:     u8 n <= 168, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Grow a dynamic array of seq_t elements to accommodate 'needed' entries. */
static void grow_seq(seq_t **arr, size_t *cap, size_t needed) {
    if (needed <= *cap) return;
    size_t new_cap = *cap == 0 ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    *arr = (seq_t *)tmp;
    *cap = new_cap;
}

/* Static storage for computed sequence and stored sums of consecutive terms. */
static seq_t *a_vals = NULL;      /* a[0], a[1], ... */
static size_t a_cap   = 0;
static size_t a_len   = 0;        /* number of elements stored */

static seq_t *sum_vals = NULL;    /* a[i] + a[i+1] for i < a_len-1 */
static size_t sum_cap  = 0;
static size_t sum_len  = 0;        /* number of sums stored */

/* Compute a(n) for any n >= 0, allowing calls in arbitrary order. */
seq_t a(idx_t n) {
    /* Ensure the initial term is present. */
    if (a_len == 0) {
        grow_seq(&a_vals, &a_cap, 1);
        a_vals[0] = 3;
        a_len = 1;
    }

    /* Grow the sequence up to index n. */
    while (a_len <= (size_t)n) {
        seq_t last = a_vals[a_len - 1];
        seq_t cand = last + 1U;

        /* Find the smallest candidate not in the sum set. */
        for (;;) {
            bool taken = false;
            for (size_t i = 0; i < sum_len; ++i) {
                if (sum_vals[i] == cand) { taken = true; break; }
            }
            if (!taken) break;
            ++cand;
        }

        /* Append the new term. */
        grow_seq(&a_vals, &a_cap, a_len + 1);
        a_vals[a_len] = cand;
        ++a_len;

        /* Add the new consecutive-sum to the set. */
        seq_t new_sum = a_vals[a_len - 2] + a_vals[a_len - 1];
        grow_seq(&sum_vals, &sum_cap, sum_len + 1);
        sum_vals[sum_len] = new_sum;
        ++sum_len;
    }

    return a_vals[(size_t)n];
}
