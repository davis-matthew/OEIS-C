/*
 * A130641  The first entry of the vector v[n]=M[n]v[n-1], where M[n] is a 7 X
 *          7 matrix M[n] = [[n, 0, 0, 0, 0, 0, 1], [1, 0, 0, 0, 0, 0, 0], [0,
 *          1, 0, 0, 0, 0, 0], [0, 0, 1, 0, 0, 0, 0], [0, 0, 0, 1, 0, 0, 0],
 *          [0, 0, 0, 0, 1, 0, 0], [0, 0, 0, 0, 0, 1, 0]] and v[0] is the
 *          column vector [0, 0, 0, 0, 0, 0, 1].
 * https://oeis.org/A130641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + a(n-7) for n >= 7, with initial values a(0)=0,
 *            a(1)=1, a(2)=2, a(3)=6, a(4)=24, a(5)=120, a(6)=720. This is the
 *            first component of the vector evolving by v[n] = M[n]v[n-1] with
 *            the given 7×7 matrix
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static size_t allocated = 0;   /* current allocation size */
    static size_t computed = 0;    /* number of elements computed */

    /* ensure the buffer is large enough for indices up to n */
    if (allocated <= (size_t)n) {
        size_t newsize = allocated ? allocated * 2 : 16;
        if (newsize <= (size_t)n) newsize = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        cache = tmp;
        allocated = newsize;
    }

    /* initialise base values on first use */
    if (computed == 0) {
        cache[0] = 0;
        cache[1] = 1;
        computed = 2;                     /* we have a[0] and a[1] ready */
    }

    /* compute missing entries up to n */
    for (size_t i = computed; i <= (size_t)n; ++i) {
        if (i <= 6) {
            /* factorial part: a[i] = i * a[i-1] */
            cache[i] = (seq_t)i * cache[i-1];
        } else {
            /* recurrence: a[i] = i*a[i-1] + a[i-7] */
            cache[i] = (seq_t)i * cache[i-1] + cache[i-7];
        }
    }
    computed = (size_t)n + 1;           /* mark up to n as computed */
    return cache[n];
}
