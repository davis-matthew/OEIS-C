/*
 * A174648  Partial sums of A000452.
 * https://oeis.org/A174648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} A000452(i)
 * Range:     u8 n <= 15, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) returns the n‑th partial sum of the greedy sequence that avoids any
   3‑term geometric progression (OEIS A174648) */

#include <stdlib.h>
#include <stdbool.h>

/* forward declaration of a helper binary search used for seq_t */
static bool binary_search_seq(const seq_t *arr, size_t len, seq_t key);

/* dynamic storage for the greedy sequence (A000452) and its cumulative sums */
static seq_t *seq_arr = NULL;   /* sequence terms */
static size_t seq_len = 0;      /* number of terms stored in seq_arr */
static size_t seq_cap = 0;      /* capacity of seq_arr */

static seq_t *cum_arr = NULL;   /* cumulative sums a(n) with a(0)=0 */
static size_t cum_len = 0;      /* number of cumulative sums stored */
static size_t cum_cap = 0;      /* capacity of cum_arr */

/* ---- utility functions ------------------------------------------------- */

/* simple binary search on a sorted array of seq_t values */
static bool binary_search_seq(const seq_t *arr, size_t len, seq_t key)
{
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (arr[mid] == key)
            return true;
        if (arr[mid] < key)
            lo = mid + 1;
        else
            hi = mid;
    }
    return false;
}

/* ensure a dynamic array has space for at least need elements */
static void ensure_capacity(size_t need, seq_t **arr, size_t *cap)
{
    if (*cap >= need)
        return;
    size_t new_cap = (*cap == 0) ? 16 : *cap << 1;
    while (new_cap < need)
        new_cap <<= 1;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

/* ---- sequence generation ---------------------------------------------- */

/* generate the greedy A000452 sequence up to n terms (1‑based) and
   fill the cumulative sum array */
static void generate_to(idx_t n)
{
    /* lazily initialise the storage arrays */
    if (seq_arr == NULL) {
        seq_arr = (seq_t *)malloc(16 * sizeof(seq_t));
        seq_cap = 16;
        seq_len = 0;
    }
    if (cum_arr == NULL) {
        cum_arr = (seq_t *)malloc(16 * sizeof(seq_t));
        cum_cap = 16;
        cum_len = 1;
        cum_arr[0] = (seq_t)0;   /* a(0) = 0 */
    }

    while (seq_len < (size_t)n) {
        /* propose the next candidate – immediately after the last term */
        seq_t candidate = (seq_len == 0) ? (seq_t)1
                                          : seq_arr[seq_len - 1] + (seq_t)1;

        /* find the smallest candidate that does not create a 3‑term
           geometric progression with any earlier pair (x, y, candidate) */
        for (;;) {
            bool bad = false;
            for (size_t i = 0; i < seq_len; ++i) {
                seq_t y      = seq_arr[i];
                seq_t y_sq   = y * y;
                if (y_sq % candidate == (seq_t)0) {
                    seq_t x = y_sq / candidate;
                    /* x < y automatically, because candidate > y */
                    if (x != y && binary_search_seq(seq_arr, seq_len, x)) {
                        bad = true;
                        break;
                    }
                }
            }
            if (!bad)
                break;
            ++candidate;
        }

        /* append the chosen term */
        ensure_capacity(seq_len + 1, &seq_arr, &seq_cap);
        seq_arr[seq_len++] = candidate;

        /* update the cumulative sum array */
        seq_t new_sum = cum_arr[cum_len - 1] + candidate;
        ensure_capacity(cum_len + 1, &cum_arr, &cum_cap);
        cum_arr[cum_len++] = new_sum;   /* cum_arr[n] holds a(n) */
    }
}

/* ---- public interface ------------------------------------------------- */

/* a(n): returns the n‑th partial sum (n >= 1) */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined case – harmless */

    generate_to(n);
    return cum_arr[n];
}
