/*
 * A187740  G.f.: Sum_{n>=0} (5*n+1)^n * x^n / (1 + (5*n+1)*x)^n.
 * https://oeis.org/A187740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n = 0 a(n) = (5*n+7) * 5^(n-1) * n! / 2 for n > 0
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* lazy memoised factorials and powers of 5 */
    static seq_t *fact = NULL;      /* fact[k] = k!      ,  k = 0 .. computed */
    static seq_t *pow5 = NULL;      /* pow5[k] = 5^k     ,  k = 0 .. computed-1 */
    static idx_t  cap    = 0;       /* allocated array size  */
    static idx_t  computed = 0;     /* highest k that has been computed */

    /* initialise on first request > 0 */
    if (n > 0 && computed == 0) {
        cap = 1;                    /* at least for k = 0 */
        fact = (seq_t *)malloc(cap * sizeof(seq_t));
        pow5 = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!fact || !pow5) {
            /* memory error – abort */
            exit(1);
        }
        fact[0] = (seq_t)1;
        pow5[0] = (seq_t)1;          /* 5^0 = 1, serves for n = 1 (pow5[n-1]) */
    }

    /* extend memoised tables if required */
    if (n > computed) {
        idx_t new_cap = cap;
        if (new_cap == 0) new_cap = 1;
        while (new_cap <= n) {
            new_cap <<= 1;          /* double until sufficient */
        }
        fact = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
        pow5 = (seq_t *)realloc(pow5, new_cap * sizeof(seq_t));
        if (!fact || !pow5) {
            /* memory error – abort */
            exit(1);
        }
        /* initialise any newly allocated elements */
        for (idx_t k = computed + 1; k <= n; ++k) {
            fact[k] = fact[k - 1] * (seq_t)k;
            pow5[k] = pow5[k - 1] * (seq_t)5;
        }
        cap = new_cap;
        computed = n;
    }

    if (n == 0) {
        return (seq_t)1;
    }

    /* a(n) = (5*n+7) * 5^(n-1) * n! / 2 */
    seq_t result = fact[n];
    result *= pow5[n - 1];
    result *= (seq_t)(5 * n + 7);
    result /= (seq_t)2;
    return result;
}
