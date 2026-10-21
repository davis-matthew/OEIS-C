/*
 * A225327  Partition numbers of the form 7k.
 * https://oeis.org/A225327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 36, u64 n <= 103, u128 n <= 385,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A225327 – partition numbers that are multiples of 7.
   The function computes ordinary partition numbers p(k) via Euler’s
   pentagonal number recurrence, keeps a list of those divisible by 7,
   and returns the n‑th such partition number (a(1)=7, a(2)=42,…). */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for realloc, malloc */

#include "../utils/seq_alloc.h"    /* seq_alloc(size_t)   */
#include "../utils/seq_realloc.h"  /* seq_realloc(ptr, old_cap, new_cap) */

/* dynamic array of partitions p(k), k = 0 … p_len‑1 */
static seq_t *partitions = NULL;
static size_t p_len      = 0;
static size_t p_cap      = 0;

/* dynamic array of partition numbers that are divisible by 7 */
static seq_t *divisible = NULL;
static size_t d_len      = 0;
static size_t d_cap      = 0;

/* Ensure that *arr has capacity >= need.
   If *arr is NULL, allocate via seq_alloc;
   otherwise call seq_realloc. */
static void ensure_capacity(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need)
        return;
    size_t new_cap = (*cap) ? (*cap) * 2 : 1;
    while (new_cap < need)
        new_cap *= 2;
    if (*cap == 0) {
        *arr = seq_alloc(new_cap);
    } else {
        *arr = seq_realloc(*arr, *cap, new_cap);
    }
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Initialise p(0) = 1 on first call */
    if (p_len == 0) {
        ensure_capacity(&partitions, &p_cap, 1);
        partitions[0] = (seq_t)1;
        p_len = 1;
    }

    /* Keep generating partition numbers until we have at least n
       divisible values stored.  Calls with larger n extend the cache. */
    while (d_len < n) {
        /* Compute p(k) for k = p_len, the next integer */
        idx_t k = (idx_t)p_len;
        seq_t total = 0;

        /* Euler’s pentagonal number theorem */
        for (idx_t m = 1; ; ++m) {
            idx_t pent1 = m * (3 * m - 1) / 2;
            if (pent1 > k)
                break;
            seq_t term = partitions[k - pent1];
            if (m & 1)
                total += term;
            else
                total -= term;

            idx_t pent2 = m * (3 * m + 1) / 2;
            if (pent2 > k)
                continue;
            term = partitions[k - pent2];
            if (m & 1)
                total += term;
            else
                total -= term;
        }

        /* Append new partition number to the array */
        ensure_capacity(&partitions, &p_cap, p_len + 1);
        partitions[p_len++] = total;

        /* If divisible by 7, add to the divisible list */
        if ((total % (seq_t)7) == 0) {
            ensure_capacity(&divisible, &d_cap, d_len + 1);
            divisible[d_len++] = total;
        }
    }

    return divisible[n - 1];
}
