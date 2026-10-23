/*
 * A132651  Sum of proper divisors of n, s(n) raised to power s(n), for n > 1.
 * https://oeis.org/A132651
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = s(n)^s(n), where s(n) is the sum of proper divisors of n
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/big_pow.h"

static idx_t precomp_max = 0;          /* largest n for which sums are ready   */
static seq_t *sum_prop   = NULL;       /* array, 1-based, sum_prop[n] = s(n)   */

static void ensure_sum_prop(idx_t n)
{
    if (n <= precomp_max) return;      /* already ready */

    /* Allocate a new array of size n+1.  Use C++ new for object
       construction when compiling as C++.  Use malloc in C. */
#ifdef __cplusplus
    seq_t *new_arr = new (std::nothrow) seq_t[n + 1];
    if (!new_arr) throw std::bad_alloc();
#else
    seq_t *new_arr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
#endif
    /* Initialise all elements to zero. */
    for (idx_t i = 0; i <= n; ++i) {
        new_arr[i] = (seq_t)0;
    }

    /* Compute the sum of proper divisors up to n using a basic
       sieve.  This recomputes the whole array but is fast for n ≤ 10000. */
    for (idx_t i = 1; i <= n / 2; ++i) {
        idx_t start = i * 2;
        for (idx_t j = start; j <= n; j += i) {
            new_arr[j] += (seq_t)i;
        }
    }

    /* Free the old array. */
#ifdef __cplusplus
    delete[] sum_prop;
#else
    free(sum_prop);
#endif
    sum_prop = new_arr;
    precomp_max = n;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* undefined for n < 2, harmless */

    ensure_sum_prop(n);
    seq_t s = sum_prop[n];
    return big_pow(s, s);
}
