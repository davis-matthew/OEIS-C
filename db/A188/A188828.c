/*
 * A188828  Number of 6 X n binary arrays without the pattern 0 1 diagonally
 *          or antidiagonally.
 * https://oeis.org/A188828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            4*a(n-1)+9*a(n-2)-45*a(n-3)+108*a(n-5)-42*a(n-6)-57*a(n-7)+18*a(n-8)+7*a(n-9)-a(n-10)
 *            for n > 11 (order‑10 linear recurrence) with initial values
 *            a(1)=64, a(2)=49, a(3)=169, a(4)=576, a(5)=2025, a(6)=7056,
 *            a(7)=24964, a(8)=87616, a(9)=310249, a(10)=1092025,
 *            a(11)=3865156
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Handle indices below the offset; they are unused. */
    if (n == 0)
        return (seq_t)0;

    /* Forward‑declared initial values for the recurrence. */
    static const seq_t init_vals[12] = {
        0,          /* dummy for n = 0 */
        64, 49, 169, 576, 2025, 7056, 24964, 87616,
        310249, 1092025, 3865156
    };

    /* Return an initial value directly for 1 ≤ n ≤ 11. */
    if (n <= 11ULL)
        return init_vals[(size_t)n];

    /* Static cache: we store computed values up to the largest n
       that has been requested so far.  The cache size is fixed at
       10 001 (enough for all n in the typical test range up to 10 000);
       the implementation can be extended by changing this constant if
       larger n are required. */
    static seq_t cache[10001];
    static size_t cached_up_to = 0;

    /* Initialise the cache with the first 11 values on the first call. */
    if (cached_up_to < 11) {
        for (size_t i = 1; i <= 11; ++i)
            cache[i] = init_vals[i];
        cached_up_to = 11;
    }

    /* Compute any missing values up to n using the recurrence. */
    size_t i;
    for (i = cached_up_to + 1; i <= (size_t)n; ++i) {
        cache[i] =
            (seq_t)4  * cache[i - 1] +
            (seq_t)9  * cache[i - 2] -
            (seq_t)45 * cache[i - 3] +
            (seq_t)108 * cache[i - 5] -
            (seq_t)42 * cache[i - 6] -
            (seq_t)57 * cache[i - 7] +
            (seq_t)18 * cache[i - 8] +
            (seq_t)7  * cache[i - 9] -
            cache[i -10];
    }
    cached_up_to = (size_t)n;

    return cache[(size_t)n];
}
