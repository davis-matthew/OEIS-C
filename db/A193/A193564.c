/*
 * A193564  In A014675, replace the n-th occurrence of 1 with n-1 and also
 *          replace the n-th occurrence of 2 with n-1.
 * https://oeis.org/A193564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 414, u16 n <= 9948, u32 n <= 9948, u64 n <= 9948,
 *            u128 n <= 9948, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(1) = 0, a(2) = 0.
 * For n >= 3, the sequence is produced by the greedy construction of the
 * Langford pair sequence: place the smallest unused integer k at the next
 * free position, and also place k at position n + k + 1.  This generates
 * the infinite sequence of Langford pairs.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static seq_t *cache_seq = NULL;
static bool *cache_filled = NULL;
static idx_t  cache_cap   = 0;
static idx_t  cache_max   = 0;
static idx_t  next_k      = 0;

static void ensure_capacity(idx_t need)
{
    if (need <= cache_cap)
        return;
    size_t old_cap = cache_cap;
    cache_cap = need > old_cap * 2 ? need : old_cap * 2;
    if (cache_cap == 0)
        cache_cap = 2;          /* start with a small positive size */

    seq_t  *new_seq    = (seq_t *)calloc(cache_cap + 1, sizeof(seq_t));
    bool    *new_filled = (bool *)calloc(cache_cap + 1, sizeof(bool));
    if (new_seq == NULL || new_filled == NULL) {
        /* Allocation failure – in a challenge environment we may just
           abort, but to stay portable we return here */
        abort();
    }
    if (cache_seq) {
        memcpy(new_seq, cache_seq, (old_cap + 1) * sizeof(seq_t));
        memcpy(new_filled, cache_filled, (old_cap + 1) * sizeof(bool));
        free(cache_seq);
        free(cache_filled);
    }
    cache_seq    = new_seq;
    cache_filled = new_filled;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined offset, return 0 */

    if (n > cache_max) {
        ensure_capacity(n);
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            if (cache_filled[i])
                continue;          /* already pre‑filled as a second occurrence */

            seq_t val = (seq_t)next_k;
            cache_seq[i]    = val;
            cache_filled[i] = true;

            idx_t second_pos = i + next_k + 1;
            ensure_capacity(second_pos);
            cache_seq[second_pos]    = val;
            cache_filled[second_pos] = true;

            next_k++;               /* move to the next integer */
        }
        cache_max = n;
    }
    return cache_seq[n];
}
