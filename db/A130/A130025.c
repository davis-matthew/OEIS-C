/*
 * A130025  a(1)=1; a(n) = a(n-1) + (number of terms, from among terms a(1)
 *          through a(n-1), which are squarefree).
 * https://oeis.org/A130025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 448, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1)=1; a(n)=a(n-1)+ (# of squarefree among a(1)..a(n-1)) */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"
#include "../utils/grow_seq.h"

static seq_t *seq_cache = NULL;   /* stores a(1), a(2), ...  (0‑based) */
static size_t cache_cap  = 0;      /* current capacity of seq_cache   */
static size_t cache_len  = 0;      /* number of terms stored          */
static size_t squarefree_cnt = 0;  /* square‑free count so far       */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* out of range guard */

    /* Expand the cache until we have at least n terms */
    while (cache_len < (size_t)n)
    {
        grow_seq(&seq_cache, &cache_cap, cache_len + 1);

        if (cache_len == 0)                /* a(1) */
        {
            seq_cache[0] = (seq_t)1;
            if (is_squarefree(seq_cache[0]))
                squarefree_cnt++;
        }
        else                               /* a(k) for k>1 */
        {
            seq_t prev = seq_cache[cache_len - 1];
            seq_t next = prev + (seq_t)squarefree_cnt;
            seq_cache[cache_len] = next;
            if (is_squarefree(next))
                squarefree_cnt++;
        }

        cache_len++;   /* now we have cache_len terms */
    }

    return seq_cache[n - 1];
}
