/*
 * A131829  Nonnegative numbers arranged so that after a(0)=0,a(1)=1, we have
 *          a(3n+2)=a(3n+1)+a(3n)-1.
 * https://oeis.org/A131829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1, a(3n+2)=a(3n+1)+a(3n)-1, a(3n), a(3n+1) are the
 *            smallest unused nonnegative integers
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Static state for memoisation and bookkeeping */
static seq_t *seq_cache = NULL;
static idx_t  seq_len   = 0;
static idx_t  seq_cap   = 0;

static bool *used = NULL;
static idx_t  used_cap = 0;

/* Global counter for the next candidate unused number */
static seq_t next_unused = 0;

/* Ensure the boolean array can index up to 'max_val' */
static void ensure_used_capacity(seq_t max_val)
{
    /* max_val should be representable as idx_t */
    idx_t need = (idx_t)max_val + 1u;
    if (need <= used_cap) return;

    idx_t new_cap = used_cap ? used_cap * 2 : 16;
    if (new_cap < need) new_cap = need;

    bool *new_arr = (bool *)realloc(used, new_cap * sizeof(bool));
    if (!new_arr) abort();          /* allocation failure */
    /* initialise new entries to false */
    for (idx_t i = used_cap; i < new_cap; ++i) new_arr[i] = false;
    used = new_arr;
    used_cap = new_cap;
}

/* Ensure the sequence cache can hold at least 'len' elements */
static void ensure_seq_capacity(idx_t len)
{
    if (len <= seq_cap) return;
    idx_t new_cap = seq_cap ? seq_cap * 2 : 16;
    if (new_cap < len) new_cap = len;
    seq_t *new_arr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_arr) abort();          /* allocation failure */
    seq_cache = new_arr;
    seq_cap = new_cap;
}

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* Grow cache up to n */
    if (seq_len <= n) {
        ensure_seq_capacity(n + 1);
        for (idx_t i = seq_len; i <= n; ++i) {
            seq_t value;
            if (i == 0) {
                value = (seq_t)0;
            } else if (i == 1) {
                value = (seq_t)1;
            } else if ((i % 3u) == 2u) /* i % 3 == 2 */
            {
                /* recurrence */
                value = seq_cache[i-1] + seq_cache[i-2] - (seq_t)1;
            } else
            {
                /* smallest unused nonnegative integer */
                while (used[(idx_t)next_unused])
                    ++next_unused;
                value = next_unused;
                ++next_unused;
            }
            seq_cache[i] = value;
            /* Mark the value as used */
            ensure_used_capacity(value);
            used[(idx_t)value] = true;
        }
        seq_len = n + 1;
    }
    return seq_cache[n];
}
