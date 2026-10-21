/*
 * A087917  Number of unordered ways to write n as a sum of 3 odious numbers
 *          (A000069).
 * https://oeis.org/A087917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i+j+k = n} A010060(i)*A010060(j)*A010060(k) (count
 *            of unordered triples of odd‑popcount numbers summing to n)
 * Range:     u8 n <= 150, u16 n <= 2478, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_odious_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_odious_uint64.h"

/* ------------------------------------------------------------------ */
/*  Cached data about odious (odd popcount) numbers up to a growing    */
/*  bound.  The arrays are lazily expanded as larger n are requested. */
static bool *odious_flag  = NULL;   /* odious_flag[i] == true iff i is odious */
static idx_t odious_flag_size = 0;  /* number of entries allocated in odious_flag */
static idx_t *odious_list    = NULL;/* list of odious numbers in ascending order */
static idx_t odious_list_cnt = 0;   /* number of entries in odious_list */

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/*  Extend the odious data structures so that all indices up to n      */
/*  are known.  This function is thread‑unsafe but the evaluator does  */
/*  not use concurrent calls. */
static void ensure_odious_up_to(idx_t n)
{
    if (odious_flag_size > n)
        return; /* already sufficient */

    /* --- Allocate / enlarge the boolean flag array --- */
    bool *new_flag = (bool *)realloc(odious_flag,
                                     (size_t)(n + 1) * sizeof(bool));
    if (!new_flag)
        return; /* allocation failed – keep old data */
    odious_flag = new_flag;

    /* initialise newly added entries (indices [odious_flag_size..n]) */
    for (idx_t i = odious_flag_size; i <= n; ++i)
        odious_flag[i] = is_odious_uint64(i);

    /* --- Allocate / enlarge the list of odious numbers --- */
    size_t new_array_size = (size_t)(n + 1);                  /* capacity in elems */
    if (!odious_list) {                                      /* first allocation */
        odious_list = (idx_t *)malloc(new_array_size * sizeof(idx_t));
        if (!odious_list)
            return;
    } else {                                                  /* resize */
        idx_t *tmp = (idx_t *)realloc(odious_list,
                                      new_array_size * sizeof(idx_t));
        if (!tmp)
            return;
        odious_list = tmp;
    }

    /* Append any new odious numbers to the list, preserving order. */
    idx_t next_start = odious_list_cnt ? odious_list[odious_list_cnt - 1] + 1
                                       : 0;
    for (idx_t i = next_start; i <= n; ++i)
        if (odious_flag[i])
            odious_list[odious_list_cnt++] = i;

    odious_flag_size = new_array_size;
}

/* ------------------------------------------------------------------ */
/*  Sequence function: count unordered triples of odious numbers that   */
/*  sum to n.  The function can be called with arbitrary n in any order.*/
seq_t a(idx_t n)
{
    if (n < 1U)
        return 0;                     /* offset is 1 – undefined below 1 */

    /* make sure we know ordios up to n */
    ensure_odious_up_to(n);

    seq_t count = 0;

    /* iterate over unordered triples i <= j <= k, all odious */
    for (idx_t ii = 0; ii < odious_list_cnt; ++ii) {
        idx_t i_val = odious_list[ii];
        if (i_val > n)
            break;

        /* j starts from i */
        for (idx_t jj = ii; jj < odious_list_cnt; ++jj) {
            idx_t j_val = odious_list[jj];
            if (j_val > n)
                break;

            /* if i + j already exceeds n, no further j will work */
            if (i_val > n - j_val)
                break;

            idx_t sum_ij = i_val + j_val;
            if (sum_ij > n)
                break;

            idx_t k_val = n - sum_ij;
            if (k_val < j_val)          /* enforce j <= k */
                continue;

            if (k_val <= n && odious_flag[k_val])
                ++count;                /* a valid unordered triple found */
        }
    }

    return count;
}
