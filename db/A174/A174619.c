/*
 * A174619  a(n) = A174618(n) + A174618(n+1).
 * https://oeis.org/A174619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2) - a(n-4) + a(n-8), initial values: a(1)=1,
 *            a(2)=1, a(3)=1, a(4)=2, a(5)=2, a(6)=3, a(7)=3, a(8)=4 (sequence
 *            offset 1)
 * Range:     u8 n <= 25, u16 n <= 48, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* holds a[1] … a[cached_n] (a[0] unused) */
static idx_t cached_n = 0;       /* highest index currently stored */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* sequence is undefined for n<1, return 0 */

    /* initialise cache with the first 8 terms */
    if (cache == NULL) {
        cache = (seq_t*)calloc(9, sizeof(seq_t));   /* indices 0 … 8 */
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 1;
        cache[4] = 2;
        cache[5] = 2;
        cache[6] = 3;
        cache[7] = 3;
        cache[8] = 4;
        cached_n = 8;
    }

    if (n <= cached_n)          /* already computed */
        return cache[n];

    /* grow the table to n */
    cache = (seq_t*)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));

    /* compute new terms using the recurrence */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        /* for i >= 9 the recurrence is defined */
        seq_t val = cache[i - 2];
        val += val;                 /* val = 2*a(i-2) */
        val -= cache[i - 4];        /* val = 2*a(i-2) - a(i-4) */
        val += cache[i - 8];        /* val = 2*a(i-2) - a(i-4) + a(i-8) */
        cache[i] = val;
    }
    cached_n = n;
    return cache[n];
}
