/*
 * A104741  First terms in the rearrangements of integer numbers (see
 *          comments).
 * https://oeis.org/A104741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the first element after n−1 moves of the rule: current
 *            first element r is moved to immediately after the r‑th prime
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code keeps a mutable representation of the infinite list and memoizes
   the head (first element) after each step, so that repeated calls with
   arbitrary n are answered in O(n−current_max) time. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#ifndef MAX_R_INDEX
#  define MAX_R_INDEX 200000          /* number of r‑th primes to precompute */
#endif

#ifndef SIEVE_LIMIT
#  define SIEVE_LIMIT 3500000         /* enough to collect MAX_R_INDEX primes */
#endif

/* global state variables */
static bool     initialized = false;
static int     *primes_by_index = NULL;   /* 1‑based: primes_by_index[1] = 2 */
static int      prime_count = 0;

static int     *next_arr = NULL;          /* next[i]  : i→next value */
static int     *prev_arr = NULL;          /* prev[i]  : i←prev value */
static int       head = 1;                /* current first element */

static seq_t  *head_at = NULL;            /* head_at[n] = first element after n−1 moves */
static idx_t    head_at_cap = 0;
static idx_t    head_at_size = 0;         /* highest n stored (1‑based) */
static idx_t    current_step = 1;         /* highest n simulated so far */

/* utility to ensure the head_at table has room for index n */
static void
ensure_head_at_cap(idx_t n)
{
    if (n + 1 <= head_at_cap)
        return;
    idx_t new_cap = head_at_cap ? head_at_cap * 2 : 128;
    while (new_cap < n + 1)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!tmp)
        return;                           /* memory failure: give up */
    if (head_at) {
        memcpy(tmp, head_at, head_at_size * sizeof(seq_t));
        free(head_at);
    }
    head_at = tmp;
    head_at_cap = new_cap;
}

static void
init_everything(void)
{
    if (initialized)
        return;
    initialized = true;

    /* ------------ generate first MAX_R_INDEX primes ----------------- */
    int limit = SIEVE_LIMIT;
    unsigned char *is_comp = (unsigned char *)malloc((size_t)(limit + 1));
    if (!is_comp)
        return;
    memset(is_comp, 0, (size_t)(limit + 1));
    int root = (int)sqrt((double)limit);
    for (int i = 2; i <= root; ++i) {
        if (!is_comp[i]) {
            for (int j = i * i; j <= limit; j += i)
                is_comp[j] = 1;
        }
    }
    primes_by_index = (int *)malloc((size_t)(MAX_R_INDEX + 1) * sizeof(int));
    if (!primes_by_index) {
        free(is_comp);
        return;
    }
    prime_count = 0;
    for (int i = 2; i <= limit && prime_count < MAX_R_INDEX; ++i) {
        if (!is_comp[i]) {
            ++prime_count;
            primes_by_index[prime_count] = i;
        }
    }
    free(is_comp);

    /* ---------- allocate linked list arrays ------------------------- */
    int val_lim = limit + 1;               /* include prime values up to limit */
    next_arr = (int *)malloc((size_t)(val_lim + 2) * sizeof(int));
    prev_arr = (int *)malloc((size_t)(val_lim + 2) * sizeof(int));
    if (!next_arr || !prev_arr) {
        /* clean up */
        free(primes_by_index);
        if (next_arr) free(next_arr);
        if (prev_arr) free(prev_arr);
        return;
    }
    for (int i = 1; i < val_lim; ++i) {
        next_arr[i] = i + 1;
        prev_arr[i] = i - 1;
    }
    next_arr[val_lim - 1] = 0;   /* sentinel for "no next" */
    next_arr[val_lim] = 0;
    prev_arr[1] = 0;

    /* ----- initialize head_at memoisation table ------- */
    head_at = (seq_t *)malloc(128 * sizeof(seq_t));
    if (!head_at)
        return;
    head_at_cap = 128;
    head_at_size = 1;
    head_at[1] = (seq_t)head;
}

/* ------------------------------------------------------------------- */
/* Main function as required                                                */
/* ------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_everything();
    if (n < 1)
        return (seq_t)0;

    if (n <= current_step)
        return head_at[n];

    /* extend simulation up to n */
    while (current_step < n) {
        /* perform one move */
        int r = head;                     /* value to move */
        if (r < 1 || r > prime_count)    /* safety: should not happen */
            return (seq_t)0;
        int prime_val = primes_by_index[r];

        /* remove r from front */
        int new_head = next_arr[head];
        if (new_head != 0)
            prev_arr[new_head] = 0;
        head = new_head;

        /* insert r after prime_val */
        int after = next_arr[prime_val];
        next_arr[prime_val] = r;
        prev_arr[r] = prime_val;
        next_arr[r] = after;
        if (after != 0)
            prev_arr[after] = r;

        /* record new head */
        ++current_step;
        ensure_head_at_cap(current_step);
        head_at[current_step] = (seq_t)head;
    }

    return head_at[n];
}
