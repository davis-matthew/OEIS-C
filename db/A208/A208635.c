/*
 * A208635  Number of n X 6 0..1 arrays with new values 0..1 introduced in row
 *          major order and no element equal to more than one of its immediate
 *          leftward or upward or right-upward antidiagonal neighbors.
 * https://oeis.org/A208635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 15*a(n-2) + 10*a(n-3) - a(n-4). Initial
 *            conditions: a(1)=32, a(2)=95, a(3)=309, a(4)=1042
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the nth term of OEIS sequence A208635 (offset 1). */
seq_t a(idx_t n)
{
    /* Handle out‑of‑range request. */
    if (n == 0)
        return 0;

    /* Static cache storing already computed values. */
    static seq_t *cache = NULL;   /* array of a(k) for k=1..last */
    static size_t cap  = 0;       /* allocated capacity (elements)   */
    static idx_t last = 0;        /* highest n already computed      */

    /* Grow the cache if necessary. */
    if (cap < (size_t)n) {
        size_t new_cap = (cap == 0) ? 4 : cap * 2;
        if (new_cap < (size_t)n)
            new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();               /* allocation failure  */
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise the first four terms if they are not yet set. */
    if (last < 1) { cache[0] = 32; last = 1; }
    if (last < 2) { cache[1] = 95; last = 2; }
    if (last < 3) { cache[2] = 309; last = 3; }
    if (last < 4) { cache[3] = 1042; last = 4; }

    /* Compute any missing terms using the recurrence. */
    for (idx_t ii = last + 1; ii <= n; ++ii) {
        size_t idx = (size_t)ii - 1;       /* zero‑based index for a(ii) */
        seq_t a1 = cache[idx - 1];         /* a(ii-1)   */
        seq_t a2 = cache[idx - 2];         /* a(ii-2)   */
        seq_t a3 = cache[idx - 3];         /* a(ii-3)   */
        seq_t a4 = cache[idx - 4];         /* a(ii-4)   */

        seq_t val = (seq_t)7 * a1;
        val -= (seq_t)15 * a2;
        val += (seq_t)10 * a3;
        val -= a4;
        cache[idx] = val;
    }

    last = n;
    return cache[(size_t)n - 1];
}
