/*
 * A132217  Expansion of psi(x^6) / psi(-x) in powers of x where psi() is a
 *          Ramanujan theta function.
 * https://oeis.org/A132217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 55, u32 n <= 163, u64 n <= 537,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of partitions of n into parts not
 * congruent to 0, 2 or 10 (mod 12).  We compute it by a
 * standard 1‑dimensional partition DP, recomputing the
 * table only when a larger n is requested.
 */
#include <stddef.h>
#include <stdlib.h>

static seq_t *dp = NULL;         /* array of accumulated values */
static idx_t   dp_max = (idx_t)-1;/* highest index computed so far */

/* ------------------------------------------------------------------ */
/* Test whether a part size part is allowed.  Parts congruent to
 * 0, 2 or 10 (mod 12) are excluded. */
static inline int allowed_part(size_t part)
{
    size_t r = part % 12;
    return r != 0 && r != 2 && r != 10;
}

/* ------------------------------------------------------------------ */
/* Allocate an array of size len, zero‑initialised. */
static seq_t *allocate_seq_array(size_t len)
{
#ifdef __cplusplus
    return new seq_t[len]();   /* value‑initialise */
#else
    return (seq_t *)calloc(len, sizeof(seq_t));
#endif
}

/* ------------------------------------------------------------------ */
/* Free an array allocated by allocate_seq_array. */
static void free_seq_array(seq_t *p)
{
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

/* ------------------------------------------------------------------ */
/* Return a(n), building the DP table up to n if necessary. */
seq_t a(idx_t n)
{
    /* 1. If the cache is empty, initialise it. */
    if (dp == NULL) {
        dp = allocate_seq_array(1);
        dp[0] = 1;          /* a(0) = 1 */
        dp_max = 0;
    }

    /* 2. Expand the cache if a larger index is requested. */
    if ((idx_t)dp_max < n) {
        /* Allocate a new array large enough for the new limit. */
        seq_t *new_dp = allocate_seq_array((size_t)n + 1);
        if (!new_dp) {         /* allocation failed – return 0 */
            return (seq_t)0;
        }

        /* Compute the partition numbers up to n. */
        new_dp[0] = 1;         /* base case */

        for (size_t part = 1; part <= (size_t)n; ++part) {
            if (!allowed_part(part)) continue;

            for (size_t i = part; i <= (size_t)n; ++i) {
                new_dp[i] += new_dp[i - part];
            }
        }

        /* Replace the old cache. */
        free_seq_array(dp);
        dp = new_dp;
        dp_max = n;
    }

    /* 3. Return the value for the requested index. */
    return dp[(size_t)n];
}
