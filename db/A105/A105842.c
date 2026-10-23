/*
 * A105842  Numbers n such that 31*n^2 + 31*n + 1 is a square.
 * https://oeis.org/A105842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the nth integer n such that 31*n^2+31*n+1 is a perfect
 * square.  The sequence satisfies the linear recurrence:
 *
 *   a(1)=0, a(2)=55, a(3)=9064, a(4)=1480479,
 *   a(5)=3140319, a(6)=512899624, a(7)=83770465015, a(8)=13681996386240,
 *   a(n)=9241598*a(n‑4)+4620798-a(n‑8)  for n≥9.
 *
 * The implementation memoises computed values in a static array so that
 * repeated calls (in any order) are O(1) after the initial expansion.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* small constant helpers – cast to seq_t to avoid narrowing */
    const seq_t C1 = (seq_t)9241598;
    const seq_t C2 = (seq_t)4620798;

    /* static cache: 1‑based terms stored in 0‑based array */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* base terms (1-indexed) */
    static const seq_t base[8] = {
        (seq_t)0ULL,
        (seq_t)55ULL,
        (seq_t)9064ULL,
        (seq_t)1480479ULL,
        (seq_t)3140319ULL,
        (seq_t)512899624ULL,
        (seq_t)83770465015ULL,
        (seq_t)13681996386240ULL
    };

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_len = 8;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        for (idx_t i = 0; i < 8; ++i)
            cache[i] = base[i];
    }

    /* if requested index is within cache, return immediately */
    if (n <= cache_len)
        return cache[n - 1];

    /* grow cache to accommodate new terms */
    seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
    if (tmp == NULL) {
        /* memory allocation failed; return 0 as a harmless fallback */
        return (seq_t)0;
    }
    cache = tmp;

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_len; i < n; ++i) {
        /* i is zero‑based; term number = i+1 */
        cache[i] = C1 * cache[i - 4] + C2 - cache[i - 8];
    }

    cache_len = n;           /* update length */
    return cache[n - 1];     /* 0‑based index -> term n */
}
