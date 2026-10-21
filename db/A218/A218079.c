/*
 * A218079  Number of n X 3 arrays of the minimum value of corresponding
 *          elements and their horizontal, vertical or antidiagonal neighbors
 *          in a random 0..1 n X 3 array.
 * https://oeis.org/A218079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2) - 5*a(n-4) - a(n-5) for n > 6 Initial
 *            values: a(1)=4, a(2)=8, a(3)=26, a(4)=78, a(5)=246, a(6)=762 The
 *            sequence counts the fixed points of the “min‑over‑neighbors” map
 *            on n×3 binary arrays. We compute terms on demand and memoize
 *            them so that calls in any order run in O(...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *memo = NULL;   /* cached values, 1‑based */
    static idx_t cap   = 0;      /* allocated size */
    static idx_t last  = 0;      /* largest n for which memo is computed */

    if (n == 0) return 0; /* safety guard – offset is 1 */

    /* Grow the cache if needed */
    if (n > cap) {
        idx_t newcap = cap ? cap * 2 : 16;
        while (newcap < n) newcap *= 2;
        memo = (seq_t*)realloc(memo, (newcap + 1) * sizeof(seq_t));
        for (idx_t i = cap + 1; i <= newcap; ++i) memo[i] = 0;
        cap = newcap;
    }

    /* Initialise the first six terms once */
    if (memo[1] == 0) {
        memo[1] = 4;  memo[2] = 8;  memo[3] = 26;
        memo[4] = 78; memo[5] = 246; memo[6] = 762;
    }

    /* Compute missing values up to n */
    if (last < n) {
        for (idx_t i = last + 1; i <= n; ++i) {
            if (i <= 6) continue;          /* already initialized */
            seq_t val = 2 * memo[i-1] + 4 * memo[i-2];
            val -= 5 * memo[i-4];
            val -= memo[i-5];
            memo[i] = val;
        }
        last = n > last ? n : last;
    }

    return memo[n];
}
