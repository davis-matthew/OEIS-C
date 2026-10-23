/*
 * A028580  C_{n+1}*C_n*C_{n-1}^2*C_{n-2}^4*...*C_2^{2^(n-2)}, where C_k are
 *          the Catalan numbers (A000108).
 * https://oeis.org/A028580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(2*n-1)*a(n-1)^2/(n+1), a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t computed = 0;

    /* safe guard for offset < 1 */
    if (n < 1) return (seq_t)0;

    /* enlarge cache if necessary */
    if (cache_size <= n) {
        idx_t new_size = n + 1;              /* we use indices 1..n, index 0 unused */
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* out of memory */
        cache_size = new_size;
    }

    /* initialise first value if not already done */
    if (computed < 1) {
        cache[1] = (seq_t)1;
        computed = 1;
    }

    /* compute missing terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t prev  = cache[i - 1];
        seq_t num   = prev * prev;                               /* a(i-1)^2 */
        seq_t mult  = (seq_t)(2 * (2 * i - 1));                   /* 2*(2*i-1) */
        num *= mult;                                             /* numerator */
        seq_t denom = (seq_t)(i + 1);                             /* i+1 */
        cache[i] = num / denom;                                  /* exact division */
    }

    computed = n;
    return cache[n];
}
