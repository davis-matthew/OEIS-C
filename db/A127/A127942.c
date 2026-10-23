/*
 * A127942  a(n) = denominator of b(n), where b(1)=1, b(n) = Sum_{1<=k<n,
 *          gcd(k,n)=1} 1/b(k).
 * https://oeis.org/A127942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   * a(n) = denominator of b(n) where * b(1) = 1 * b(n) = Σ_{1≤k<n,
 *            gcd(k,n)=1} 1/b(k) * * This implementation memoises the rational
 *            values b(k) as numerator * and denominator in two parallel
 *            static arrays. The gcd routine * comes from the shared utility
 *            header "../utils/gcd.h". No * additional util...
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 8, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>   /* for size_t */
#include "../utils/gcd.h"

static seq_t *b_num = NULL;   /* numerator of b(k) */
static seq_t *b_den = NULL;   /* denominator of b(k) */
static size_t cache_size = 0; /* allocated size in elements */
static idx_t  max_computed = 0; /* largest n already computed */

static void
ensure_cache(idx_t n)
{
    if ((idx_t)cache_size <= n) {
        size_t new_size = (size_t)(n + 1);          /* indices 0..n inclusive */
        b_num = (seq_t *)realloc(b_num, new_size * sizeof(seq_t));
        b_den = (seq_t *)realloc(b_den, new_size * sizeof(seq_t));
        /* Newly allocated elements are uninitialised – they will be
           written when needed.  */
        cache_size = new_size;
    }
}

/* Add term = term_num / term_den to the current sum sum_num / sum_den.
 * The result is reduced by GCD to keep the numbers small. */
static void
add_fraction(seq_t *sum_num, seq_t *sum_den,
             seq_t term_num, seq_t term_den)
{
    seq_t new_num = (*sum_num) * term_den + term_num * (*sum_den);
    seq_t new_den = (*sum_den) * term_den;
    seq_t g = gcd(new_num, new_den);          /* gcd from the shared utilities */
    *sum_num = new_num / g;
    *sum_den = new_den / g;
}

/* Return a(n) = denominator of b(n).  Works for any order of calls. */
seq_t a(idx_t n)
{
    if (n == 0u)               /* undefined; return 1 as a benign value */
        return (seq_t)1;

    ensure_cache(n);

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i == 1u) {
            b_num[1] = (seq_t)1;
            b_den[1] = (seq_t)1;
            continue;
        }

        seq_t sum_num = (seq_t)0;
        seq_t sum_den = (seq_t)1;

        for (idx_t k = 1; k < i; ++k) {
            if (gcd((seq_t)k, (seq_t)i) == 1) {
                /* term = 1 / b(k) = b_den[k] / b_num[k] */
                add_fraction(&sum_num, &sum_den,
                             b_den[k], b_num[k]);
            }
        }

        b_num[i] = sum_num;
        b_den[i] = sum_den;
    }

    max_computed = n;
    return b_den[n];
}
