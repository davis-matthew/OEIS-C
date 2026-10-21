/*
 * A091650  Let M = the 4 X 4 matrix [0 1 0 0 / 0 0 1 0 / 0 0 0 1 / -1 -1 3
 *          2]. Set seed vector = [1 1 1 1] = first row, then take M*[1 1 1 1]
 *          = [1 1 1 3] then M * [1 1 1 3], etc. Sequence gives terms in
 *          rightmost column.
 * https://oeis.org/A091650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) - a(n-3) - a(n-4) with a(1)=1,
 *            a(2)=3, a(3)=7, a(4)=21
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for size_t, required for array indices */

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1 – behaviour for n<1 is irrelevant */
        return 0;

    /* Static cache to memoise computed terms.  A size of 20001 covers
       n up to about 10 000, which is the required range. */
    static seq_t cache[20001];
    static idx_t max_precomputed = 0;

    /* Initialise the first four terms on first entry */
    if (max_precomputed == 0) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)7;
        cache[4] = (seq_t)21;
        max_precomputed = 4;
    }

    /* If requested term is already cached, return it immediately */
    if (n <= max_precomputed)
        return cache[n];

    /* Compute terms up to n using the recurrence.  All intermediate
       results stay within seq_t because the recurrence involves only
       additions and subtractions of already computed terms. */
    for (idx_t i = max_precomputed + 1; i <= n; ++i) {
        cache[i] =
            (seq_t)2 * cache[i - 1] +
            (seq_t)3 * cache[i - 2] -
            cache[i - 3] -
            cache[i - 4];
    }
    max_precomputed = n;
    return cache[n];
}
