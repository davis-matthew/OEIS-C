/*
 * A138892  Non-Ulam numbers.
 * https://oeis.org/A138892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Non-Ulam numbers – numbers that are NOT in the Ulam sequence A002858. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ------- Internal state ------- */
static bool initialized = false;

/* Ulam numbers array */
static seq_t *ulam_arr = NULL;
static size_t ulam_len = 0;
static size_t ulam_cap = 0;

/* Counts of representations as sums of two distinct Ulam numbers.
   cnt[s] == number of ways s = u_i + u_j (i < j) with u_i,u_j previously seen. */
static int *cnt = NULL;
static idx_t max_sum = 0;

/* Non‑Ulam numbers array */
static seq_t *nonulam_arr = NULL;
static size_t nonulam_len = 0;
static size_t nonulam_cap = 0;

/* Next integer to test for being Ulam or non‑Ulam */
static idx_t next_candidate = 3;   /* 1 and 2 are the first Ulam numbers */

/* ------- Helper functions ------- */
static void ensure_ulam_cap(size_t need)
{
    if (need <= ulam_cap) return;
    size_t new_cap = ulam_cap ? ulam_cap * 2 : 8;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(ulam_arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    ulam_arr = tmp;
    ulam_cap = new_cap;
}

static void ensure_nonulam_cap(size_t need)
{
    if (need <= nonulam_cap) return;
    size_t new_cap = nonulam_cap ? nonulam_cap * 2 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(nonulam_arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    nonulam_arr = tmp;
    nonulam_cap = new_cap;
}

static void ensure_cnt_cap(idx_t need)
{
    if (need <= max_sum) return;
    size_t cur = (size_t)(max_sum + 1);
    size_t new_cap = cur ? cur * 2 : 64;
    if ((idx_t)new_cap < need) new_cap = (size_t)need + 1;
    int *tmp = (int *)realloc(cnt, new_cap * sizeof(int));
    if (!tmp) abort();
    for (size_t i = cur; i < new_cap; ++i) tmp[i] = 0;
    cnt = tmp;
    max_sum = new_cap - 1;
}

static void add_ulam(seq_t val)
{
    ensure_ulam_cap(ulam_len + 1);
    ulam_arr[ulam_len++] = val;
}

static void add_nonulam(seq_t val)
{
    ensure_nonulam_cap(nonulam_len + 1);
    nonulam_arr[nonulam_len++] = val;
}

/* Update representation counts with the new Ulam number */
static void update_counts_with_new_ulam(seq_t new_val)
{
    for (size_t i = 0; i < ulam_len - 1; ++i) {
        idx_t sum = (idx_t)ulam_arr[i] + (idx_t)new_val;
        ensure_cnt_cap(sum);
        ++cnt[sum];
    }
}

/* ------- Public function ------- */
seq_t a(idx_t n)
{
    if (!initialized) {
        initialized = true;

        /* Initialize Ulam numbers 1, 2 */
        add_ulam((seq_t)1);
        add_ulam((seq_t)2);
        /* Account for sum 1+2 = 3 */
        ensure_cnt_cap(3);
        ++cnt[3];

        /* First non‑Ulam is 0 */
        add_nonulam((seq_t)0);
    }

    /* Generate numbers until we have the requested non‑Ulam */
    while (nonulam_len <= n) {
        ensure_cnt_cap(next_candidate);
        if (cnt[next_candidate] == 1) {
            /* New Ulam number */
            add_ulam((seq_t)next_candidate);
            update_counts_with_new_ulam((seq_t)next_candidate);
        } else {
            /* Non‑Ulam number */
            add_nonulam((seq_t)next_candidate);
        }
        ++next_candidate;  /* move to next integer */
    }

    return nonulam_arr[n];
}
