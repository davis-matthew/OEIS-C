/*
 * A184542  Number of (n+2) X 5 binary arrays with each 3 X 3 subblock having
 *          rows and columns in lexicographically nondecreasing order.
 * https://oeis.org/A184542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 18, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A184542: Number of (n+2)×5 binary arrays with every 3×3 subblock
   having rows and columns in lexicographically nondecreasing order.
   The sequence satisfies the linear recurrence
   a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3) - 15*a(n-4) + 6*a(n-5) - a(n-6)
   for n > 6, with initial values (n = 1 … 6):
   a(1)=147  a(2)=340  a(3)=631  a(4)=1048  a(5)=1627  a(6)=2413
*/
#include <stdlib.h>
#include <stdbool.h>

/* multiply a by a small integer constant k (0 ≤ k ≤ 20) using only
   addition to avoid overflow on small seq_t types                                 */
static seq_t mul_const(seq_t a, unsigned int k)
{
    seq_t r = 0;
    for (unsigned int i = 0; i < k; ++i)
        r += a;
    return r;
}

/* Static cache: cache[i] holds a(i) for i ≥ 1.
   cache[0] is unused. cached_n is the largest index that has been computed. */
static seq_t *cache = NULL;
static idx_t  cached_n = 0;   /* 0 means nothing computed yet */

/* Compute a(n) for n ≥ 1, using memoised values and the recurrence. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined for n == 0 */

    /* Ensure the array is large enough for indices up to n. */
    size_t needed = (size_t)n + 1;      /* indices 0 … n */
    if (needed < 7) needed = 7;         /* need space for the 6 initial values */

    if (cache == NULL) {
        cache = (seq_t *)realloc(NULL, needed * sizeof(seq_t));
        if (!cache) abort();           /* allocation failure */
        cache[0] = 0;                   /* unused entry */

        /* Initialise the first 6 terms. */
        cache[1] = 147;
        cache[2] = 340;
        cache[3] = 631;
        cache[4] = 1048;
        cache[5] = 1627;
        cache[6] = 2413;
        cached_n = 6;
    } else if (needed > (size_t)cached_n + 1) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
    }

    /* Compute any missing terms up to n using the recurrence. */
    idx_t start = (cached_n < 6) ? 6 : cached_n;   /* start from the first missing index */
    start = (start < 6) ? 6 : start;
    for (idx_t i = start + 1; i <= n; ++i) {
        seq_t s_pos = mul_const(cache[i-1], 6);
        s_pos += mul_const(cache[i-3], 20);
        s_pos += mul_const(cache[i-5], 6);

        seq_t s_neg = mul_const(cache[i-2], 15);
        s_neg += mul_const(cache[i-4], 15);
        s_neg += cache[i-6];

        cache[i] = s_pos - s_neg;   /* result is positive */
    }
    if (n > cached_n) cached_n = n;

    return cache[n];
}
