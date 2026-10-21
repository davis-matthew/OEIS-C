/*
 * A214188  Number of 0..6 colorings of a 1 X (n+1) array circular in the n+1
 *          direction with new values 0..6 introduced in row major order.
 * https://oeis.org/A214188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n-1) - 79*a(n-2) + 165*a(n-3) - 64*a(n-4) * -
 *            180*a(n-5) + 144*a(n-6) * Initial values: * a(1)=1, a(2)=1,
 *            a(3)=4, a(4)=11, a(5)=41, a(6)=162 * This function memoises the
 *            values up to the largest n requested * so any order of calls is
 *            supported. It works with any seq_t type * provided...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 16, u64 n <= 28, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>     /* for size_t */

seq_t a(idx_t n)
{
    /* Cache array: index 0 is unused to keep the recurrence indices
       directly aligned with n.  The array is big enough for all
       values requested up to the stated limit (10 000). */
    static seq_t cache[10001 + 1];
    static idx_t cache_len = 0;   /* highest n computed so far */

    /* Initialise the first six terms on the first call. */
    if (cache_len == 0) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)4;
        cache[4] = (seq_t)11;
        cache[5] = (seq_t)41;
        cache[6] = (seq_t)162;
        cache_len = 6;
    }

    /* Compute missing terms up to n if needed. */
    if (n > cache_len) {
        for (idx_t k = cache_len + 1; k <= n; ++k) {
            /* Recurrence over the last six values. */
            seq_t a1 = cache[k - 1];
            seq_t a2 = cache[k - 2];
            seq_t a3 = cache[k - 3];
            seq_t a4 = cache[k - 4];
            seq_t a5 = cache[k - 5];
            seq_t a6 = cache[k - 6];

            /* Compute using only arithmetic operators. */
            seq_t term = (seq_t)15 * a1
                       - (seq_t)79 * a2
                       + (seq_t)165 * a3
                       - (seq_t)64 * a4
                       - (seq_t)180 * a5
                       + (seq_t)144 * a6;

            cache[k] = term;
        }
        cache_len = n;
    }

    return cache[n];
}
