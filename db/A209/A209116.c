/*
 * A209116  Number of 4-bead necklaces labeled with numbers -n..n not allowing
 *          reversal, with sum zero and avoiding the patterns z z+1 z+2 and z
 *          z-1 z-2.
 * https://oeis.org/A209116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 36, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A209116: Number of 4‑bead necklaces labeled with numbers –n..n not
 * allowing reversal, with sum zero and avoiding the patterns
 *   z, z+1, z+2  and  z, z–1, z–2.
 *
 * Empirical recurrence (OEIS):
 *   a(n) = 3*a(n-1) – 3*a(n-2) + 2*a(n-3) – 3*a(n-4) + 3*a(n-5) – a(n-6)
 *   Initial values (1‑based):
 *     a(1)=5,  a(2)=22,  a(3)=55,  a(4)=120,  a(5)=221,  a(6)=362.
 *
 * The function memoises values in a static array so that calls can be made
 * in any order.  It works for all integer types specified for seq_t and
 * idx_t and for arbitrary‑precision builds (when compiled as C++17).
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;    /* memoisation buffer */
static size_t cache_len = 0;   /* number of entries stored so far */

/*--------------------------------------------------------------------
 *  Return the n-th term of A209116.  n ≥ 1.
 *--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Convert idx_t to unsigned size_t for indexing */
    size_t target = (size_t)n;   /* target index (1‑based) */

    /* Ensure array large enough to hold entries up to 'target' */
    if (target > cache_len) {
        size_t new_len = target;                           /* might be larger than current */
        seq_t *p = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!p && new_len > 0)                            /* allocation failed */
            return (seq_t)0;
        cache = p;
        /* We will fill new entries next */
    }

    /* Compute missing terms up to 'target' */
    for (size_t i = cache_len; i < target; ++i) {           /* i = n-1 (0‑based) */
        size_t k = i + 1;                                  /* 1‑based index */

        if (k <= 6) {                                      /* base cases */
            static const seq_t base[6] = {5, 22, 55, 120, 221, 362};
            cache[i] = base[k-1];
        } else {
            /* Apply the recurrence:
             * a(k) = 3*a(k-1) – 3*a(k-2) + 2*a(k-3) – 3*a(k-4) + 3*a(k-5) – a(k-6)
             */
            seq_t term =  (seq_t)3 * cache[i-1]             /* 3*a(k-1) */
                       - (seq_t)3 * cache[i-2]             /* –3*a(k-2) */
                       + (seq_t)2 * cache[i-3]             /* +2*a(k-3) */
                       - (seq_t)3 * cache[i-4]             /* –3*a(k-4) */
                       + (seq_t)3 * cache[i-5]             /* +3*a(k-5) */
                       - cache[i-6];                      /* –a(k-6) */
            cache[i] = term;
        }
    }
    cache_len = target;   /* update the stored length */

    return cache[target-1];
}
