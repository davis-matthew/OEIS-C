/*
 * A092462  n times the coefficient of x^n in log[1 + sum(k>=0, x^2^k)].
 * https://oeis.org/A092462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n * [x^n] log(1 + ∑_{k≥0} x^{2^k})
 * Range:     i8 n <= 23, i16 n <= 51, i32 n <= 52, i64 n <= 52, i128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation uses the reciprocal of 1 + ∑x^{2^k}.  r[0]=1, r[n] = -∑_{p∈P2≤n} r[n-p] */
/* Then a(n) = ∑_{p∈P2≤n} p * r[n-p]  (for n>0).  a(0)=0.  P2 = {1,2,4,8,…}. */

/* Requires: <stdlib.h> for malloc/realloc, <stdint.h> for uint64_t.
   Types idx_t and seq_t are provided by the build environment. */

#include <stdlib.h>
#include <stdint.h>

/* static cache of r[n] values (coefficients of 1/(1+∑x^{2^k})) */
static seq_t *r_cache = NULL;
static idx_t r_max_cached = 0;

/* compute a(n) */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    /* Extend cache up to n if necessary */
    if (n > r_max_cached) {
        /* Allocate or grow array to size n+1 */
        seq_t *new_cache = (seq_t *)realloc(r_cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;      /* out of memory */
        r_cache = new_cache;

        /* Initialize first element if this is the first call */
        if (r_max_cached == 0) {
            r_cache[0] = (seq_t)1;
        }

        /* Compute r[i] for i = r_max_cached+1 .. n */
        for (idx_t i = r_max_cached + 1; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            for (uint64_t pow = 1; pow <= i; pow <<= 1) {
                sum += r_cache[i - pow];
                if (pow > i / 2) break;        /* avoid overflow of pow <<=1 */
            }
            r_cache[i] = -sum;
        }
        r_max_cached = n;
    }

    /* Compute a(n) using the cached r values */
    seq_t result = (seq_t)0;
    for (uint64_t pow = 1; pow <= n; pow <<= 1) {
        result += (seq_t)pow * r_cache[n - pow];
        if (pow > n / 2) break;                /* prevent overflow shift */
    }
    return result;
}
