/*
 * A231258  Number of (n+1) X (3+1) 0..2 arrays with no element unequal to a
 *          strict majority of its horizontal and antidiagonal neighbors, with
 *          values 0..2 introduced in row major order.
 * https://oeis.org/A231258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 17*a(n-1) - 96*a(n-2) + 308*a(n-3) - 628*a(n-4) +
 *            800*a(n-5) - 656*a(n-6) + 256*a(n-7) (n ≥ 8)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The first seven terms are:
    a(1)=4,   a(2)=32,   a(3)=304,   a(4)=2982,
    a(5)=29366, a(6)=289230, a(7)=2848550.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1 */

    /* Static cache of computed terms.   */
    static seq_t *cache = NULL;
    static idx_t max_cached = 0;   /* highest n for which cache[n] is valid */

    /* Initialise cache on first call.  */
    if (!cache) {
        size_t cap = (size_t)(n >= 7 ? n + 1 : 8); /* at least indices 1..7 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;              /* allocation failed */

        /* First seven terms */
        cache[1] = 4;          cache[2] = 32;          cache[3] = 304;
        cache[4] = 2982;       cache[5] = 29366;       cache[6] = 289230;
        cache[7] = 2848550;
        max_cached = 7;
    }

    if (n <= max_cached) return cache[n];

    /* Grow cache to accommodate up to n.   */
    size_t new_cap = (size_t)(n + 1);   /* indices 0..n inclusive */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return (seq_t)0;          /* allocation failed */
    cache = tmp;

    /* Compute terms from (max_cached + 1) up to n using the recurrence. */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t term = (seq_t)17 * cache[i-1];
        term -= (seq_t)96 * cache[i-2];
        term += (seq_t)308 * cache[i-3];
        term -= (seq_t)628 * cache[i-4];
        term += (seq_t)800 * cache[i-5];
        term -= (seq_t)656 * cache[i-6];
        term += (seq_t)256 * cache[i-7];
        cache[i] = term;
    }

    max_cached = n;
    return cache[n];
}
