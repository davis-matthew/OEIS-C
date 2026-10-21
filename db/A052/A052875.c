/*
 * A052875  E.g.f.: (exp(x)-1)^2/(2-exp(x)).
 * https://oeis.org/A052875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{i=2..n} i! * S(n,i) (equal to the Fubini number
 *            A000670(n) minus 1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* static buffer holding Fubini numbers f[k] = A000670(k) */
    static seq_t *f = NULL;
    static size_t f_cap = 0;
    static idx_t max_computed = 0;   /* largest k for which f[k] is known */

    /* initialise */
    if (f == NULL) {
        f_cap = 1;
        f = (seq_t *)malloc(f_cap * sizeof(seq_t));
        if (!f) return (seq_t)0;     /* allocation failure: return 0 */
        f[0] = (seq_t)1;              /* A000670(0) = 1 */
        max_computed = 0;
    }

    /* extend the array if needed */
    if ((size_t)n + 1 > f_cap) {
        size_t new_cap = f_cap ? 2 * f_cap : 1;
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;
        f = (seq_t *)realloc(f, new_cap * sizeof(seq_t));
        f_cap = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k < i; ++k) {
            seq_t bin = binom((idx_t)i, (idx_t)k);  /* binomial coefficient (i choose k) */
            sum += bin * f[k];
        }
        f[i] = sum;   /* f[i] = A000670(i) */
    }
    max_computed = n;

    /* return a(n) = f[n] - 1, turning 0 for n = 0 or 1 into the correct value */
    return f[n] - (seq_t)1;
}
