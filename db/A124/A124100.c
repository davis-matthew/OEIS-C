/*
 * A124100  Sum_(x^i*y^j*z^k) with i + j + k = m and (x, y, z) = the primitive
 *          Pythagorean triple (8, 15, 17).
 * https://oeis.org/A124100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 40*a(n-1) - 511*a(n-2) + 2040*a(n-3) with a(0) = 1, a(1)
 *            = 40, a(2) = 1089
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for memoisation */
    static seq_t *cache = NULL;  /* array of computed terms */
    static idx_t  len   = 0;     /* number of terms stored */
    static idx_t  cap   = 0;     /* allocated capacity */

    /* initialise cache with the first three terms (if not yet done) */
    if (cache == NULL) {
        cap   = 8;                    /* arbitrary initial capacity */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (cache == NULL)           /* allocation failure – unlikely in tests */
            return (seq_t)0;

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)40;
        cache[2] = (seq_t)1089;
        len     = 3;
    }

    /* compute terms up to n when needed */
    while (len <= n) {
        /* double the capacity if we hit the limit */
        if (len == cap) {
            idx_t new_cap = cap * 2;
            if (new_cap == 0) new_cap = 8;   /* safety for zero (unlikely) */
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL)           /* allocation failure */
                return (seq_t)0;
            cache = tmp;
            cap   = new_cap;
        }

        /* recurrence: a(k) = 40*a(k-1) - 511*a(k-2) + 2040*a(k-3) */
        seq_t val = 40 * cache[len-1];
        val -= 511 * cache[len-2];
        val += 2040 * cache[len-3];

        cache[len] = val;
        len++;
    }

    return cache[n];
}
