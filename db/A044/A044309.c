/*
 * A044309  Numbers k such that string 6,4 occurs in the base 9 representation
 *          of n but not of k-1.
 * https://oeis.org/A044309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044309:
   numbers k such that the base‑9 representation of k contains the substring
   "64" but the base‑9 representation of k‑1 does not.  The function
   returns the n‑th such number (n≥1). */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
#include <new>
#endif

/* Helper that reports whether a base‑9 number contains the substring 6,4. */
static bool has64(seq_t x)
{
    /* We traverse the digits from least significant to most significant.
       For digits D[i] (low=0) we look at the pair (upper, lower) where
       lower = D[i] and upper = D[i+1].  Substring 6,4 means upper==6 and
       lower==4. */
    while (x >= 9u)
    {
        seq_t lower = x % 9u;
        seq_t upper = (x / 9u) % 9u;
        if (upper == 6u && lower == 4u)
            return true;
        x /= 9u;
    }
    return false;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Static storage for memoisation. */
    static seq_t *cache = NULL;
    static size_t capacity = 0;     /* allocated size of cache   */
    static idx_t count = 0;         /* number of terms computed */
    static seq_t cur = 0;           /* last integer examined    */

    /* Ensure we have the n‑th term cached. */
    while (count < n)
    {
        /* Examine the next integer. */
        cur++;
        if (has64(cur) && !has64(cur - 1))
        {
            /* Need to extend the cache if necessary. */
            if ((size_t)count >= capacity)
            {
                size_t newCap = capacity ? capacity * 2 : 16;
                while (newCap <= (size_t)count)
                    newCap *= 2;

#ifdef __cplusplus
                seq_t *tmp = new seq_t[newCap];
#else
                seq_t *tmp = (seq_t *)malloc(newCap * sizeof(seq_t));
#endif
                if (tmp == NULL)
                    return 0; /* allocation failure – unlikely for our limits */

                /* Copy old data. */
                for (size_t i = 0; i < (size_t)count; ++i)
                    tmp[i] = cache[i];

#ifdef __cplusplus
                delete[] cache;
#else
                free(cache);
#endif
                cache = tmp;
                capacity = newCap;
            }
            /* Store the new term. */
            cache[count] = cur;
            count++;
        }
    }
    return cache[n - 1];
}
