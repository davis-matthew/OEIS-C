/*
 * A232031  Number of (n+1) X (1+1) 0..1 arrays with every element equal to
 *          some horizontal, vertical or antidiagonal neighbor, with top left
 *          element zero.
 * https://oeis.org/A232031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) + a(n-3) - a(n-4) initial values:
 *            a(1)=3, a(2)=11, a(3)=37, a(4)=129
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the trivial cases directly */
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)11;
    if (n == 3) return (seq_t)37;
    if (n == 4) return (seq_t)129;

    /* Cache for memoisation */
    static seq_t *seq = NULL;
    static idx_t cache_max = 0;

    /* Initialise cache if not yet done */
    if (cache_max == 0) {
        size_t sz = 5;                     /* indices 0..4 used */
        seq = (seq_t*)malloc(sz * sizeof(seq_t));
        if (!seq) return (seq_t)0;         /* allocation failure – unlikely */
        seq[1] = (seq_t)3;
        seq[2] = (seq_t)11;
        seq[3] = (seq_t)37;
        seq[4] = (seq_t)129;
        cache_max = 4;
    }

    /* Return cached value if already computed */
    if (n <= cache_max) return seq[n];

    /* Grow the cache to accommodate n */
    size_t new_sz = (size_t)(n + 1);
    seq = (seq_t*)realloc(seq, new_sz * sizeof(seq_t));
    if (!seq) return (seq_t)0;              /* allocation failure */

    /* Compute new terms using the recurrence */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq[i] = (seq_t)4 * seq[i - 1]
               - (seq_t)2 * seq[i - 2]
               +   seq[i - 3]
               -   seq[i - 4];
    }

    cache_max = n;
    return seq[n];
}
