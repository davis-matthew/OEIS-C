/*
 * A115793  Integers i such that i XOR 10i = 11i.
 * https://oeis.org/A115793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 1791, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A115793: Integers i such that i XOR (10*i) = 11*i.
   An n‑th term is obtained by testing successive integers until the
   condition holds.  Terms are cached in a static array so that any
   order of calls works.  The algorithm is O(n) in the number of terms
   required and uses only integer arithmetic on the native idx_t type. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache    = NULL;   /* stored terms */
    static idx_t  count    = 0;      /* number of terms stored */
    static idx_t  capacity = 0;      /* allocated capacity */
    static idx_t  next     = 0;      /* next integer candidate */

    if (count < n) {
        /* enlarge capacity if needed */
        if (capacity == 0)
            capacity = 16;
        else
            while (n > capacity)
                capacity <<= 1;      /* double until enough */

        if (cache == NULL)
            cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        else
            cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));

        if (cache == NULL)
            abort();  /* allocation failed */

        /* search for remaining terms */
        while (count < n) {
            idx_t val = next;                       /* candidate integer */
            /* use native idx_t arithmetic for the test */
            if ((val ^ (val * 10)) == (val * 11))
                cache[count++] = (seq_t)val;        /* store term */
            ++next;                                 /* advance candidate */
        }
    }

    return cache[n - 1];   /* 1‑based indexing */
}
