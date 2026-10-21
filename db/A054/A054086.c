/*
 * A054086  For k >= 1, let p(k)=least h in N not already an a(i),
 *          q(k)=p(k)+k, a(3k-1)=q(k), a(3k)=p(k), a(3k+1)=least h in N not
 *          already an a(i).
 * https://oeis.org/A054086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, eigen
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   grow_used
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A054086
 * Formula: a(1) = 1; for k ≥ 1:
 *   p(k) = least natural number not yet appearing in the sequence
 *   q(k) = p(k) + k
 *   a(3k-1) = q(k),  a(3k)   = p(k),
 *   a(3k+1) = least natural number not yet appearing after the above two
 * The function implements this recurrence and memoises the sequence.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/grow_used.h"

static void grow_cache(size_t need, seq_t **cache, size_t *cache_cap)
{
    if (*cache_cap >= need)
        return;
    size_t new_cap = *cache_cap ? *cache_cap : 32;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(*cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        exit(1);
    *cache = new_cache;
    *cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* below offset – any value is acceptable */
        return 0;

    /* static state: cached terms and used numbers */
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static size_t seq_len = 0;

    static unsigned char *used = NULL;
    static size_t used_cap = 0;

    static idx_t min_unused = 1;     /* smallest natural number not yet used */

    /* initialise static structures on first call */
    if (!seq) {
        seq_cap = 32;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) exit(1);
        seq[0] = 1;                  /* a(1) = 1 */
        seq_len = 1;

        used_cap = 64;
        used = (unsigned char *)calloc(used_cap, 1);
        if (!used) exit(1);
        used[1] = 1;                  /* number 1 is used */
    }

    /* generate terms until we have at least n entries */
    while (seq_len < (size_t)n) {
        /* current block index k : sequence length after a block = 3k+1 */
        idx_t k = ((idx_t)seq_len - 1) / 3 + 1;

        /* find smallest unused number -> p(k) */
        grow_used((size_t)min_unused + 1, &used, &used_cap);
        while (used[(size_t)min_unused])
            min_unused++;
        idx_t p = min_unused;
        used[(size_t)min_unused] = 1;  /* mark p(k) as used */

        /* compute q(k) = p(k) + k */
        idx_t q_idx = p + k;
        grow_used((size_t)q_idx + 1, &used, &used_cap);

        /* allocate room for three new terms */
        grow_cache(seq_len + 3, &seq, &seq_cap);

        /* a(3k-1) = q(k) */
        seq[seq_len++] = (seq_t)q_idx;
        /* a(3k)   = p(k) */
        seq[seq_len++] = (seq_t)p;
        used[(size_t)q_idx] = 1;       /* mark q(k) as used */

        /* a(3k+1) = next unused number */
        grow_used((size_t)min_unused + 1, &used, &used_cap);
        while (used[(size_t)min_unused])
            min_unused++;
        seq[seq_len++] = (seq_t)min_unused;
        used[(size_t)min_unused] = 1;   /* mark this number */
    }

    return seq[n - 1];
}
