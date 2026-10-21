/*
 * A084335  a(1) = 1; a(n+1) is the least composite number m not already used
 *          such that |m-a(n)| is not equal to |a(k+1)-a(k)| for any k < n.
 * https://oeis.org/A084335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 4; a(n+1) is the least unused composite number m such that
   |m-a(n)| has not occurred as a successive difference before. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/is_prime_small.h"   /* bool is_prime_small(idx_t) */

/* cache for computed sequence entries */
static seq_t *seq      = NULL;
static idx_t  seq_len  = 0;        /* number of terms stored (1‑based) */
static size_t seq_cap  = 0;        /* allocated capacity */

/* bitmap for composites already used */
static uint8_t *used      = NULL;  /* 0 = not used, 1 = used */
static size_t   used_cap  = 0;     /* allocated capacity (indexable up to used_cap-1) */

/* bitmap for differences that have already appeared */
static uint8_t *diff_used = NULL;  /* 0 = not seen, 1 = seen */
static size_t   diff_cap  = 0;     /* allocated capacity */

/* Ensure seq array can hold at least 'need' elements. */
static void ensure_seq_capacity(size_t need)
{
    if (need <= seq_cap) return;
    size_t newcap = seq_cap ? seq_cap * 2 : 32;
    while (newcap < need) newcap *= 2;
    seq = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
    seq_cap = newcap;
}

/* Ensure used bitmap can index up to 'maxIdx' (inclusive). */
static void ensure_used_capacity(size_t maxIdx)
{
    if (maxIdx < used_cap) return;
    size_t newcap = used_cap ? used_cap * 2 : 64;
    while (newcap <= maxIdx) newcap *= 2;
    uint8_t *tmp = (uint8_t *)realloc(used, newcap);
    memset(tmp + used_cap, 0, newcap - used_cap);
    used = tmp;
    used_cap = newcap;
}

/* Ensure diff_used bitmap can index up to 'maxIdx' (inclusive). */
static void ensure_diff_capacity(size_t maxIdx)
{
    if (maxIdx < diff_cap) return;
    size_t newcap = diff_cap ? diff_cap * 2 : 64;
    while (newcap <= maxIdx) newcap *= 2;
    uint8_t *tmp = (uint8_t *)realloc(diff_used, newcap);
    memset(tmp + diff_cap, 0, newcap - diff_cap);
    diff_used = tmp;
    diff_cap = newcap;
}

/* Compute the n-th term of A084335 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, harmless */

    /* Grow the cache up to n terms */
    while (seq_len < n)
    {
        /* --- initialise a(1) --------------------------------- */
        if (seq_len == 0)
        {
            ensure_seq_capacity(1);
            ensure_used_capacity(4);
            seq[0] = (seq_t)4;
            seq_len = 1;
            used[4] = 1;                  /* mark composite 4 as used   */
            continue;
        }

        /* --- find the next term --------------------------------- */
        seq_t prev = seq[seq_len - 1];

        /* search composite numbers in increasing order */
        for (idx_t candidate = 4; ; ++candidate)
        {
            /* need to store the candidate in used bitmap */
            if (candidate < 4) continue;  /* safety */

            ensure_used_capacity((size_t)candidate);

            /* skip primes and already used composites */
            if (is_prime_small((idx_t)candidate) || used[(size_t)candidate])
                continue;

            /* compute absolute difference */
            seq_t diff = (candidate > prev) ? (candidate - prev) : (prev - candidate);
            size_t diff_idx = (size_t)diff;

            allow_num: /* label for flow e.g., when we might want to skip */
            ensure_diff_capacity(diff_idx);
            if (!diff_used[diff_idx])
            {
                /* we have found the next term */
                ensure_seq_capacity(seq_len + 1);
                seq[seq_len++] = candidate;
                used[(size_t)candidate] = 1;
                diff_used[diff_idx] = 1;
                break;
            }
        }
    }

    return seq[n - 1];
}
