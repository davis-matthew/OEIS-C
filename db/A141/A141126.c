/*
 * A141126  a(n+1) = a(n)-n^2 if positive and new, else a(n+1) = a(n)+n^2;
 *          a(0)=1.
 * https://oeis.org/A141126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 Recurrence: for n ≥ 0 let n2 = n². If a(n) > n2 and
 *            (a(n) – n2) has not appeared earlier, a(n+1) = a(n) – n2, else
 *            a(n+1) = a(n) + n2
 * Range:     u8 n <= 16, u16 n <= 240, u32 n <= 9728, u64 n <= 9728,
 *            u128 n <= 9728, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Global cache for already computed sequence values */
static seq_t *seq_cache   = NULL;
static idx_t  seq_cap     = 0;
static idx_t  seq_len     = 0;

/* Ensure the cache can hold at least 'need' elements */
static void ensure_capacity(idx_t need)
{
    if (need <= seq_cap) return;

    idx_t newcap = seq_cap ? seq_cap << 1 : 8;
    while (newcap < need) newcap <<= 1;

    seq_t *tmp = (seq_t *)realloc(seq_cache, newcap * sizeof(seq_t));
    if (!tmp) exit(1);                /* abort on allocation failure */
    seq_cache = tmp;
    seq_cap   = newcap;
}

/* Linear search to check if 'value' has already been seen */
static bool seen_in_cache(seq_t value)
{
    for (idx_t i = 0; i < seq_len; ++i)
        if (seq_cache[i] == value) return true;
    return false;
}

/* Sequence function */
seq_t a(idx_t n)
{
    /* Initialise cache on first call */
    if (seq_len == 0) {
        ensure_capacity(1);
        seq_cache[0] = (seq_t)1;   /* a(0)=1 */
        seq_len = 1;
    }

    /* Compute terms up to index n */
    while (seq_len <= n) {
        idx_t cur_idx = seq_len - 1;            /* current n */
        seq_t cur_val = seq_cache[cur_idx];
        seq_t n2 = (seq_t)cur_idx * (seq_t)cur_idx;

        seq_t next_val;
        if (cur_val > n2) {
            seq_t candidate = cur_val - n2;
            next_val = seen_in_cache(candidate) ? cur_val + n2 : candidate;
        } else
            next_val = cur_val + n2;

        ensure_capacity(seq_len + 1);
        seq_cache[seq_len++] = next_val;
    }

    return seq_cache[n];
}
