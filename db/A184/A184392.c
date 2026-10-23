/*
 * A184392  a(n) is the product of palindromic divisors of n.
 * https://oeis.org/A184392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = product of the palindromic divisors of n (n ≥ 1, a(1) =
 *            1)
 * Range:     u8 n <= 17, u16 n <= 43, u32 n <= 197, u64 n <= 2663,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Check whether an unsigned integer is a decimal palindrome. */
static bool is_palindrome_idx(idx_t x)
{
    idx_t rev = 0;
    idx_t tmp = x;
    while (tmp > 0) {
        rev = rev * 10 + tmp % 10;
        tmp /= 10;
    }
    return rev == x;
}

/* Cache of already‑computed values.  cache[0] is unused. */
static seq_t *cache = NULL;      /* dynamic array of seq_t */
static idx_t cached_max = 0;     /* largest n for which cache[n] is valid */

/* Compute the product of the palindromic divisors for all n up to the
   requested value, then return a(n). */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;           /* undefined for n=0 */

    /* Expand the cache if necessary. */
    if (n > cached_max) {
        size_t new_size = (size_t)n + 1;   /* 1‑based index */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();                 /* allocation failed */
        cache = tmp;

        for (idx_t i = cached_max + 1; i <= n; ++i) {
            seq_t prod = (seq_t)1;
            for (idx_t d = 1; d * d <= i; ++d) {
                if (i % d == 0) {
                    if (is_palindrome_idx(d))
                        prod *= (seq_t)d;
                    idx_t d2 = i / d;
                    if (d2 != d && is_palindrome_idx(d2))
                        prod *= (seq_t)d2;
                }
            }
            cache[i] = prod;
        }
        cached_max = n;
    }

    return cache[n];
}
