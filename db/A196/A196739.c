/*
 * A196739  a(n) = n! - n^10.
 * https://oeis.org/A196739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = factorial(n) - n^10
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 8, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   factorial, pow_uint, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial.h"
#include "../utils/pow_uint.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* Cache factorial values so that repeated calls are fast. */
    static seq_t *fac = NULL;
    static idx_t fac_len = 0;   /* current allocated length */
    static idx_t fac_last = 0;  /* largest n for which fac[n] is computed */

    if (fac == NULL) {                     /* first call: allocate and init */
        fac_len = n + 1;                   /* at least needed length */
        fac = seq_alloc(fac_len);
        fac[0] = (seq_t)1;                 /* 0! = 1 */
        fac_last = 0;
    }

    /* Grow the cache if required and compute new factorials. */
    if (n > fac_last) {
        idx_t new_len = n + 1;
        if (new_len > fac_len) {
            fac = seq_realloc(fac, fac_len, new_len);
            fac_len = new_len;
        }
        for (idx_t i = fac_last + 1; i <= n; ++i) {
            fac[i] = fac[i - 1] * (seq_t)i;
        }
        fac_last = n;
    }

    seq_t fact = fac[n];
    seq_t power = pow_uint((uint64_t)n, (idx_t)10);
    return fact - power;
}
