/*
 * A238461  Greatest number k such that A000009(k) <= n.
 * https://oeis.org/A238461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max{k: A000009(k) <= n}
 * Range:     u8 n <= 247, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A000009(k) is the number of partitions of k into distinct parts. */

#include <stdlib.h>
#include <string.h>

static seq_t *dp = NULL;            /* DP array for partitions */
static size_t dp_cap = 0;
static idx_t  dp_len = 0;           /* last part processed */

static seq_t *Qvec = NULL;          /* Q(k) values stored as Q(k) = Qvec[k-1] */
static size_t Qcap = 0;
static idx_t  Qlen = 0;

/* Ensure Qvec contains values up to a value strictly greater than limit. */
static void ensure_up_to_value(seq_t limit)
{
    /* Initialising the DP array if it hasn't been done yet. */
    if (dp_cap == 0) {
        dp_cap = 64;
        dp = (seq_t *)calloc(dp_cap, sizeof(seq_t));
        dp[0] = 1;
        dp_len = 0;
    }
    /* Compute new Q(k) values until we have one > limit. */
    while (Qlen == 0 || Qvec[Qlen-1] <= limit) {
        idx_t next_part = dp_len + 1;

        /* Grow DP array if necessary. */
        if ((idx_t)dp_cap <= next_part) {
            size_t newcap = dp_cap ? dp_cap * 2 : 64;
            while ((idx_t)newcap <= next_part) newcap *= 2;
            seq_t *ptr = (seq_t *)realloc(dp, newcap * sizeof(seq_t));
            if (!ptr) exit(1);          /* abort on allocation failure */
            /* Zero the new entries. */
            memset(ptr + dp_cap, 0, (newcap - dp_cap) * sizeof(seq_t));
            dp = ptr;
            dp_cap = newcap;
        }

        /* Update DP for the new part (distinct parts -> loop descending). */
        for (idx_t s = dp_cap - 1; s >= next_part; --s) {
            dp[s] += dp[s - next_part];
            if (s == next_part) break;   /* avoid unsigned underflow */
        }

        dp_len = next_part;
        seq_t qk = dp[next_part];          /* Q(k) for current k */

        /* Grow Q vector if needed. */
        if ((size_t)Qlen >= Qcap) {
            size_t newcap = Qcap ? Qcap * 2 : 32;
            seq_t *ptr = (seq_t *)realloc(Qvec, newcap * sizeof(seq_t));
            if (!ptr) exit(1);
            Qvec = ptr;
            Qcap = newcap;
        }
        Qvec[Qlen++] = qk;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* out of offset, undefined behaviour */

    /* Ensure we have computed all Q(k) up to the first one > n. */
    ensure_up_to_value((seq_t)n);

    /* Binary search for largest k with Q(k) <= n.  Q(k) = Qvec[k-1]. */
    idx_t low = 1;                 /* minimal possible k is 1 */
    idx_t high = Qlen;             /* at least one Q > n exists */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (Qvec[mid-1] <= (seq_t)n)
            low = mid;
        else
            high = mid - 1;
    }
    return (seq_t)low;
}
