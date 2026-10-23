/*
 * A039500  Iterations of "k->k/2 if k is even, k->3k-1 if k is odd" (A001281)
 *          starting at these numbers reach 1.
 * https://oeis.org/A039500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 421, u32 n <= 8373, u64 n <= 8373,
 *            u128 n <= 8373, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers that reach 1 under the modified Collatz iteration
   (k -> k/2 if k even, k -> 3k-1 if k odd).  For each n ≥ 1,
   a(n) is the n‑th such positive integer. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* See that status[0] is unused; indexes correspond to the value k.
   0 = unknown, 1 = reaches 1, 2 = does not reach 1 (loops). */
static unsigned char *status = NULL;
static idx_t status_limit = 0;

/* Array of the discovered good numbers, 1-indexed (good_list[0] = a(1)). */
static seq_t *good_list = NULL;
static size_t good_cap = 0;
static idx_t good_count = 0;
static idx_t next_candidate = 1;

/* Ensure the status array can hold values up to n (inclusive). */
static void ensure_status_limit(idx_t n)
{
    if (n <= status_limit) return;
    idx_t new_limit = n * 2 + 1024;
    if (new_limit < 1024) new_limit = 1024;
    unsigned char *tmp = (unsigned char *)realloc(status, (size_t)(new_limit + 1));
    if (!tmp) exit(1);
    /* Zero the newly allocated part. */
    for (idx_t i = status_limit + 1; i <= new_limit; ++i) tmp[i] = 0;
    status = tmp;
    status_limit = new_limit;
}

/* Test whether a starting value x eventually reaches 1.  Uses memoization. */
static bool reaches_one(seq_t x)
{
    if (x == 1) return true;
    idx_t ix = (idx_t)x;
    if (ix > status_limit) ensure_status_limit(ix);
    if (ix <= status_limit && status[ix] == 1) return true;
    if (ix <= status_limit && status[ix] == 2) return false;

    seq_t path[256];
    idx_t path_len = 0;
    seq_t cur = x;

    while (1) {
        if (cur == 1) {
            /* Mark entire path as good */
            for (idx_t i = 0; i < path_len; ++i) {
                idx_t idx = (idx_t)path[i];
                if (idx <= status_limit) status[idx] = 1;
            }
            return true;
        }
        idx_t ci = (idx_t)cur;
        if (ci <= status_limit) {
            if (status[ci] == 1) {
                for (idx_t i = 0; i < path_len; ++i) {
                    idx_t idx = (idx_t)path[i];
                    if (idx <= status_limit) status[idx] = 1;
                }
                return true;
            }
            if (status[ci] == 2) {
                for (idx_t i = 0; i < path_len; ++i) {
                    idx_t idx = (idx_t)path[i];
                    if (idx <= status_limit) status[idx] = 2;
                }
                return false;
            }
        }

        /* Detect a cycle within the current path. */
        for (idx_t i = 0; i < path_len; ++i) {
            if (path[i] == cur) {
                for (idx_t j = i; j < path_len; ++j) {
                    idx_t idx = (idx_t)path[j];
                    if (idx <= status_limit) status[idx] = 2;
                }
                for (idx_t j = 0; j < i; ++j) {
                    idx_t idx = (idx_t)path[j];
                    if (idx <= status_limit) status[idx] = 2;
                }
                return false;
            }
        }

        if (path_len == 256) {
            /* Path too long – assume it fails for safety. */
            for (idx_t i = 0; i < path_len; ++i) {
                idx_t idx = (idx_t)path[i];
                if (idx <= status_limit) status[idx] = 2;
            }
            return false;
        }
        path[path_len++] = cur;

        /* Next step of the modified Collatz. */
        if (cur & 1) {
            cur = 3 * cur - 1;
        } else {
            cur = cur / 2;
        }
    }
}

/* Ensure good_list can hold at least n elements. */
static void ensure_good_cap(idx_t n)
{
    if (n <= good_cap) return;
    size_t new_cap = good_cap > 0 ? good_cap * 2 : 16;
    while (new_cap < (size_t)n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(good_list, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    good_list = tmp;
    good_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; caller should never ask for a(0). */
    while (good_count < n) {
        idx_t cand = next_candidate++;
        if (reaches_one((seq_t)cand)) {
            ensure_good_cap(good_count + 1);
            good_list[good_count++] = (seq_t)cand;
        }
    }
    return good_list[n - 1];
}
