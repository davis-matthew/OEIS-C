/*
 * A223271  Rolling cube footprints: number of 3 X n 0..5 arrays starting with
 *          0 where 0..5 label faces of a cube and every array movement to a
 *          horizontal, diagonal or antidiagonal neighbor moves across a
 *          corresponding cube edge.
 * https://oeis.org/A223271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=36, a(2)=576, a(3)=20992, a(4)=622592 Recurrence (n>4):
 *            a(n) = 24*a(n-1) + 256*a(n-2) - 1024*a(n-3)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>    /* for size_t */

seq_t a(idx_t n)
{
    /* Cache up to n = 10000; tests are limited to this range. */
    static seq_t cache[10001];
    static idx_t computed = 0;   /* highest index that has been computed */

    /* Initialise base values once. */
    if (computed == 0) {
        cache[1] = (seq_t)36;
        cache[2] = (seq_t)576;
        cache[3] = (seq_t)20992;
        cache[4] = (seq_t)622592;
        computed = 4;
    }

    /* Return already computed value. */
    if (n <= computed)
        return cache[n];

    /* Compute missing values up to n. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        cache[i] = (seq_t)24 * cache[i-1]
                 + (seq_t)256 * cache[i-2]
                 - (seq_t)1024 * cache[i-3];
    }
    computed = n;
    return cache[n];
}
