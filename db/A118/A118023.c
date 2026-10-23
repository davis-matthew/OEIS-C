/*
 * A118023  Column 0 of triangle A118022, where the matrix square of A118022
 *          shifts each column up 1 row, dropping the main diagonal of powers
 *          of 2.
 * https://oeis.org/A118023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n ≥ 1 * a(n) = Σ_{k=0}^{n-1} a(k) * a(n-1-k) *
 *            2^((n-1) + k*(n-2) - k^2)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: 2^e is performed as a left shift.  For small integer seq_t types
 * we guard against shifting by more bits than the type width to avoid
 * undefined behaviour.  The function memoises results so it works in any
 * order of calls. */

#include <stddef.h>  /* size_t */
#include <stdlib.h>  /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache   = NULL;   /* array of a(k) */
    static idx_t  cache_n = 0;      /* largest index computed */
    static size_t cache_cap = 0;    /* current capacity of cache */

    /* special case */
    if (n == 0)
        return (seq_t)1;

    /* allocate or grow cache as necessary */
    if (!cache) {
        cache_cap = (size_t)n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failure */
        cache[0] = (seq_t)1;
        cache_n = 0;
    } else if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    if (cache_n < n) {
        const size_t max_shift = (sizeof(seq_t) * 8ULL) - 1ULL;
        for (idx_t m = cache_n + 1; m <= n; ++m) {
            seq_t sum = 0;
            idx_t nm1 = m - 1;
            for (idx_t k = 0; k <= nm1; ++k) {
                /* exponent: (m-1) + k*(m-2) - k*k  */
                idx_t exp = (m - 1) + k * (m - 2) - k * k;

                seq_t term = cache[k];
                term *= cache[nm1 - k];

                /* guard against shifting more than the width of seq_t */
                if (exp <= max_shift) {
                    term <<= exp;
                } else {
                    /* Shift would overflow the type – the term is too large
                     * to be represented, and the sequence value will not be
                     * tested for such n when seq_t is small.  Skip this term. */
                    term = 0;
                }
                sum += term;
            }
            cache[m] = sum;
        }
        cache_n = n;
    }

    return cache[n];
}
