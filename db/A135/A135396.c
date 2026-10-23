/*
 * A135396  a(n) = Product_{d|n} (n-d)!.
 * https://oeis.org/A135396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∏_{d|n} (n-d)!
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 9, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache factorials 0..max_n seen so far */
    static seq_t *fact = NULL;
    static idx_t fact_cap = 0;

    /* initialise factorial cache */
    if (fact == NULL) {
        fact_cap = 1;                       /* holds factorial(0) */
        fact = (seq_t *)malloc(sizeof(seq_t));
        if (!fact) abort();
        fact[0] = (seq_t)1;                 /* 0! = 1 */
    }

    /* extend cache to include factorial(n) if necessary */
    if (fact_cap <= n) {
        idx_t old_cap = fact_cap;
        fact_cap = n + 1;
        fact = (seq_t *)realloc(fact, (size_t)fact_cap * sizeof(seq_t));
        for (idx_t i = old_cap; i <= n; ++i) {
            fact[i] = fact[i - 1] * (seq_t)i;          /* fact[i] = fact[i‑1] * i */
        }
    }

    /* compute the product over divisors of n */
    seq_t prod = (seq_t)1;
    for (idx_t d = 1; d <= n; ++d) {
        if (n % d == 0) {
            idx_t k = n - d;                   /* k = n−d */
            prod = prod * fact[k];             /* multiply by factorial(k) */
        }
    }
    return prod;
}
