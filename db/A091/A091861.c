/*
 * A091861  a(1) = 1; a(n) = smallest positive unpicked integer such that n-k
 *          divides evenly into a(n)*a(k) for every k, 1 <= k <= n-1.
 * https://oeis.org/A091861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive unpicked integer such that for all k <
 *            n, (n-k) divides a(n) * a(k). The sequence is a permutation of
 *            the positive integers. Its values are computed incrementally and
 *            memoized
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 26, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   gcd, seq_gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/seq_gcd_seq.h"

/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* Storage for computed terms and for membership testing             */
/* --------------------------------------------------------------------- */
static seq_t *cache = NULL;      /* a(1) .. a(n)                                           */
static size_t cache_cap = 0;
static idx_t cache_len = 0;

static seq_t *used = NULL;       /* all values seen so far (unsorted)                    */
static size_t used_cap = 0;
static idx_t used_len = 0;

/* --------------------------------------------------------------------- */
/* Ensure buffers are big enough for at least n elements                 */
/* --------------------------------------------------------------------- */
static void ensure_cache(idx_t n)
{
    if ((size_t)n <= cache_cap)
        return;

    size_t new_cap = (size_t)n;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;
    cache_cap = new_cap;

    /* keep the 'used' buffer in sync (same capacity) */
    if (used_cap < new_cap) {
        seq_t *new_used = (seq_t *)realloc(used, new_cap * sizeof(seq_t));
        if (!new_used) abort();
        used = new_used;
        used_cap = new_cap;
    }
}

/* --------------------------------------------------------------------- */
/* Main sequence function                                             */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* Undefined, but should never be called */

    /* Allocate space for new terms if needed */
    ensure_cache(n);

    /* Compute terms up to n */
    for (idx_t i = cache_len; i < n; ++i) {
        idx_t idx = i + 1;              /* 1‑based index of the term being computed */

        if (idx == 1) {
            cache[i] = (seq_t)1;
            /* Record used value */
            used[used_len++] = (seq_t)1;
            continue;
        }

        /* Step 1: accumulate the lcm of the required multipliers */
        seq_t lcm_val = (seq_t)1;

        for (idx_t d = 1; d < idx; ++d) {
            /* a(index - d) is the term that must multiply a(idx) to be a multiple of d */
            seq_t b = cache[(size_t)(idx - 1 - d)];
            seq_t g1 = seq_gcd_seq((seq_t)d, b);
            seq_t m = (seq_t)d / g1;          /* multiplier needed for this d */

            seq_t g2 = seq_gcd_seq(lcm_val, m);
            lcm_val = (lcm_val / g2) * m;     /* update lcm */
        }

        /* Step 2: find the smallest multiple of lcm_val that hasn't been used yet */
        seq_t candidate = lcm_val;
        while (true) {
            bool found = false;
            for (idx_t j = 0; j < used_len; ++j) {
                if (candidate == used[j]) {
                    found = true;
                    break;
                }
            }
            if (!found)
                break;
            candidate += lcm_val;   /* next multiple */
        }

        cache[i] = candidate;
        used[used_len++] = candidate;
    }

    cache_len = n;
    return cache[n - 1];
}
