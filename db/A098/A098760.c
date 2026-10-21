/*
 * A098760  Smallest a(n) not already in the sequence and not having the same
 *          length as a(n-1).
 * https://oeis.org/A098760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = smallest unused integer not having same length as a(n-1)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   my_digit_len, pow10_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/my_digit_len.h"
#include "../utils/pow10_len.h"

/* static storage for the sequence and helper arrays */
static seq_t *seq_arr  = NULL;          /* sequence values */
static idx_t  seq_len  = 0;             /* number of stored terms */
static size_t seq_cap  = 0;             /* current capacity of seq_arr */

static seq_t *cand_len  = NULL;         /* next unused number for each length */
static seq_t *maxv_len  = NULL;         /* maximum number of each length */
static unsigned char *active_len = NULL;/* flag for active lengths */
static int            maxLen   = 1;     /* current maximum length index */
static size_t          len_cap  = 0;    /* capacity of the length arrays */

/* enlarge seq_arr if needed */
static void grow_seq(idx_t new_cap)
{
    if (seq_cap < (size_t)new_cap)
    {
        size_t cap = seq_cap ? seq_cap * 2 : 16;
        while (cap < (size_t)new_cap) cap *= 2;
        seq_arr = (seq_t*)realloc(seq_arr, cap * sizeof(seq_t));
        seq_cap = cap;
    }
}

/* enlarge length arrays if needed */
static void grow_len(idx_t new_len)
{
    if (len_cap <= (size_t)new_len)
    {
        size_t cap = len_cap ? len_cap * 2 : 4;
        while (cap <= (size_t)new_len) cap *= 2;
        cand_len   = (seq_t*)realloc(cand_len,   (cap+1) * sizeof(seq_t));
        maxv_len   = (seq_t*)realloc(maxv_len,   (cap+1) * sizeof(seq_t));
        active_len = (unsigned char*)realloc(active_len, (cap+1) * sizeof(unsigned char));
        len_cap = cap;
    }
}

/* ensure that terms up to a(n) are precomputed */
static void ensure_terms(idx_t n)
{
    if (seq_len > n) return;                 /* already have enough terms */

    /* first use: initialise storage and a(0)=0 */
    if (seq_len == 0)
    {
        grow_seq(1);
        seq_arr[0] = 0;
        seq_len = 1;

        grow_len(2);                          /* we use indices 1.. */
        active_len[1] = 1;                    /* length 1 is active */
        cand_len[1]   = 1;                    /* next unused 1‑digit number after 0 */
        maxv_len[1]   = 9;                    /* last 1‑digit number */
        maxLen = 1;
    }

    /* generate terms until index n */
    for (idx_t i = seq_len; i <= n; ++i)
    {
        int prev_len = my_digit_len(seq_arr[i-1]);

        /* find minimal candidate among active lengths ≠ prev_len */
        idx_t chosen_len = 0;
        seq_t best_val = 0;
        int found = 0;

        for (idx_t d = 1; d <= (idx_t)maxLen; ++d)
        {
            if (!active_len[d] || (int)d == prev_len)
                continue;
            seq_t cand = cand_len[d];
            if (!found || cand < best_val)
            {
                best_val = cand;
                chosen_len = d;
                found = 1;
            }
        }

        /* if none, introduce a new larger length */
        if (!found)
        {
            ++maxLen;
            grow_len((idx_t)maxLen);
            active_len[maxLen] = 1;
            cand_len[maxLen]   = pow10_len((idx_t)maxLen-1);          /* 10^(len-1) */
            maxv_len[maxLen]   = pow10_len((idx_t)maxLen) - (seq_t)1; /* 10^len - 1 */
            chosen_len = (idx_t)maxLen;
            best_val = cand_len[maxLen];
        }

        /* store the chosen term */
        grow_seq((size_t)(i+1));
        seq_arr[i] = best_val;

        /* advance the candidate for the chosen length */
        cand_len[chosen_len] += 1;
        if (cand_len[chosen_len] > maxv_len[chosen_len])
            active_len[chosen_len] = 0;

        ++seq_len;
    }
}

seq_t a(idx_t n)
{
    ensure_terms(n);
    return seq_arr[n];
}
