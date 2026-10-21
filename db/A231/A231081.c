/*
 * A231081  Number of 2 X n 0..3 arrays x(i,j) with each element horizontally,
 *          diagonally or antidiagonally next to at least one element with
 *          value (x(i,j)+1) mod 4 and at least one element with value
 *          (x(i,j)-1) mod 4, and upper left element zero.
 * https://oeis.org/A231081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0, a(2)=2, a(3)=14, a(4)=40, a(5)=356,
 *            a(n)=3*a(n-1)+12*a(n-2)+30*a(n-3)-64*a(n-4)+16*a(n-5) for n≥6
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 27, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to allow arbitrary‑order calls */
    static seq_t *cache = NULL;   /* cache[i] holds a(i); cache[0] unused */
    static idx_t cap   = 0;      /* capacity of cache (in terms) */
    static idx_t comp  = 0;      /* next index to compute; cache[1..comp-1] valid */

    if (n == 0) return 0;        /* safety, OEIS offset starts at 1 */

    /* enlarge the cache if needed */
    if ((idx_t)(n + 1) > cap) {
        idx_t new_cap = (cap > 0) ? cap : 16;
        while ((idx_t)(new_cap) < n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cap   = new_cap;
    }

    /* compute any missing terms up to n */
    idx_t start = comp ? comp : 1;            /* start from the first missing index */
    for (idx_t i = start; i <= n; ++i) {
        seq_t val;
        if (i == 1)          val = (seq_t)0;
        else if (i == 2)     val = (seq_t)2;
        else if (i == 3)     val = (seq_t)14;
        else if (i == 4)     val = (seq_t)40;
        else if (i == 5)     val = (seq_t)356;
        else
            val = (seq_t)3 * cache[i - 1]
                + (seq_t)12 * cache[i - 2]
                + (seq_t)30 * cache[i - 3]
                - (seq_t)64 * cache[i - 4]
                + (seq_t)16 * cache[i - 5];
        cache[i] = val;
    }
    comp = n + 1;        /* next free index after n */

    return cache[n];
}
