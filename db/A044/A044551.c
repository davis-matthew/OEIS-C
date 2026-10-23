/*
 * A044551  Numbers n such that string 4,4 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A044551: 
   a(n) is the n‑th integer whose base‑7 representation contains the
   consecutive digits “44” while the next integer does not. */

#include <stdbool.h>
#include <stdlib.h>

/* Test whether a number has two consecutive 4’s in base 7. */
static inline bool has44(seq_t x)
{
    seq_t prev = 6;            /* sentinel that is not 4 */
    while (x > 0)
    {
        seq_t d = x % 7;
        if (prev == 4 && d == 4)
            return true;
        prev = d;
        x /= 7;
    }
    return false;
}

/* Return the n-th term of the sequence                          */
seq_t a(idx_t n)
{
    if (n == 0)             /* just to be harmless for out‑of‑bounds calls */
        return 0;

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t next = 0;   /* next integer to test */

    if (cache == NULL)
    {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }

    while (cache_len < (size_t)n)
    {
        seq_t cand = (seq_t)next;
        bool ok = has44(cand) && !has44(cand + 1);
        next++;

        if (ok)
        {
            if (cache_len + 1 > cache_cap)
            {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len] = cand;
            ++cache_len;
        }
    }

    return cache[n - 1];
}
