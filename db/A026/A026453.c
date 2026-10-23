/*
 * A026453  a(n) = least positive integer > a(n-1) and not equal to
 *          a(i)*a(j)-2 for 1<=i<j<=n-1.
 * https://oeis.org/A026453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n) = least integer > a(n-1) such that there is no 1
 *            ≤ i < j ≤ n-1 with a(i)*a(j)-2 = a(n)
 * Range:     u8 n <= 135, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note:  The function is memoized so that calls may come in any order;
   it generates the sequence up to the requested index.  */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Thread‑local / static storage for the computed sequence. */
static seq_t *seq_data   = NULL;   /* array of sequence terms */
static size_t seq_len    = 0;       /* number of generated terms     */
static size_t seq_cap    = 0;       /* current allocated capacity    */

static seq_t *sorted_terms = NULL;  /* sorted list of all generated terms */
static size_t sorted_len   = 0;      /* number of elements in sorted_terms */
static size_t sorted_cap   = 0;      /* allocated capacity                */

/* Ensure seq_data has room for at least "needed" elements. */
static void ensure_seq_cap(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_data = (seq_t *)realloc(seq_data, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Ensure sorted_terms has room for at least "needed" elements. */
static void ensure_sorted_cap(size_t needed)
{
    if (needed <= sorted_cap) return;
    size_t new_cap = sorted_cap ? sorted_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    sorted_terms = (seq_t *)realloc(sorted_terms, new_cap * sizeof(seq_t));
    sorted_cap = new_cap;
}

/* Binary search on a sorted array.
   Returns first position with arr[pos] >= key, or len if all < key. */
static size_t binary_search(const seq_t *arr, size_t len, seq_t key)
{
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (arr[mid] < key) lo = mid + 1;
        else                hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* defined only for n ≥ 1 */

    size_t target = (size_t)n;
    /* Grow the sequence up to the requested index. */
    while (seq_len < target) {
        seq_t cand;
        if (seq_len == 0) {               /* a(1) = 1 */
            cand = 1;
        } else {
            /* Start at the next integer after the last term. */
            cand = seq_data[seq_len - 1] + 1;
            for (;;) {                    /* find the admissible value */
                seq_t target_product = cand + 2;   /* a(i)*a(j) = target_product */
                bool forbidden = false;
                for (size_t idx = 0; idx < seq_len; ++idx) {
                    seq_t ai = seq_data[idx];
                    if (target_product % ai == 0) {
                        seq_t other = target_product / ai;
                        if (other == ai) continue; /* distinct indices required */
                        /* If "other" has already appeared, the pair exists. */
                        size_t pos = binary_search(sorted_terms, sorted_len, other);
                        if (pos < sorted_len && sorted_terms[pos] == other) {
                            forbidden = true;
                            break;
                        }
                    }
                }
                if (!forbidden) break;     /* cand is admissible */
                ++cand;
            }
        }

        /* Append new term and maintain data structures. */
        ensure_seq_cap(seq_len + 1);
        seq_data[seq_len] = cand;

        ensure_sorted_cap(seq_len + 1);
        /* Find insertion position to keep sorted_terms sorted. */
        size_t insert_pos = 0;
        while (insert_pos < sorted_len && sorted_terms[insert_pos] < cand) ++insert_pos;

        /* Shift larger items to the right. */
        for (size_t k = sorted_len; k > insert_pos; --k) sorted_terms[k] = sorted_terms[k - 1];
        sorted_terms[insert_pos] = cand;

        ++seq_len;
        ++sorted_len;
    }

    return seq_data[target - 1];
}
