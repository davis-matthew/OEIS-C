/*
 * A072557  Let w(n) be defined by the following recurrence: w(1)=w(2)=w(3)=1,
 *          w(n)=(w(n-1)*w(n-2)+(w(n-1)+w(n-2))/3) / w(n-3); sequence gives
 *          values of n such that w(n) is an integer.
 * https://oeis.org/A072557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A072557: the integers n for which w(n) is an integer.
   The function uses the linear recurrence:
       a(1)=5, a(2)=11, a(3)=16, a(4)=17, a(5)=18,   a(n)=a(n-1)+a(n-5)-a(n-6)  (n>=6)
   The offset is 1 (a(1) is the first term). */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* values below the offset are unspecified; return 0 */
    if (n < 1)
        return 0;

    /* static storage for memoisation */
    static seq_t *cache = NULL;
    static idx_t  capacity = 0;
    static idx_t  max_computed = 0;   /* largest n for which cache[n] is defined */

    /* initialise on first call */
    if (cache == NULL) {
        capacity = 16;                   /* initial capacity */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (cache == NULL)
            return 0;                    /* allocation failure */

        cache[0] = (seq_t)0;            /* unused, just to be safe */
        cache[1] = (seq_t)5;             /* a(1) */
        cache[2] = (seq_t)11;            /* a(2) */
        cache[3] = (seq_t)16;            /* a(3) */
        cache[4] = (seq_t)17;            /* a(4) */
        cache[5] = (seq_t)18;            /* a(5) */
        max_computed = 5;
    }

    /* ensure the internal array can hold index n */
    if (n >= capacity) {
        idx_t new_capacity = capacity;
        while (new_capacity <= n)
            new_capacity <<= 1;          /* double until big enough */

        seq_t *tmp = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
        if (tmp == NULL)
            return 0;                   /* allocation failure */

        cache = tmp;
        capacity = new_capacity;
    }

    /* grow the cache if necessary, using the recurrence */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            /* for i <= 5 we already have the values set during init */
            if (i >= 6)
                cache[i] = cache[i - 1] + cache[i - 5] - cache[i - 6];
        }
        max_computed = n;
    }

    return cache[n];
}
