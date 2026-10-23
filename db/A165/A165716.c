/*
 * A165716  Number of tilings of a 3 X n rectangle using dominoes and right
 *          trominoes.
 * https://oeis.org/A165716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = 2*a(n-1)+6*a(n-2)-4*a(n-3)+11*a(n-4)+2*a(n-5); with
 *            a(0)=1, a(1)=0, a(2)=5, a(3)=8, a(4)=55
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base values for indices < 5 */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)8;
    if (n == 4) return (seq_t)55;

    /* Memoization storage */
    static seq_t *cache = NULL;
    static idx_t capacity = 0;   /* allocated size */
    static idx_t next = 5;       /* next index to fill (first missing) */

    /* Ensure the storage is large enough */
    if (n >= capacity) {
        idx_t new_cap = capacity? capacity << 1 : n + 1;
        if (new_cap <= n) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure */
        cache = tmp;
        capacity = new_cap;
    }

    /* Initialize first five entries once */
    if (next == 5) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)8;
        cache[4] = (seq_t)55;
    }

    /* Compute missing terms up to n */
    for (idx_t i = next; i <= n; ++i) {
        cache[i] = 2*cache[i-1] + 6*cache[i-2] - 4*cache[i-3] + 11*cache[i-4] + 2*cache[i-5];
    }
    next = n + 1;   /* update next missing index */

    return cache[n];
}
