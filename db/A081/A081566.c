/*
 * A081566  Second binomial transform of expansion of exp(3cosh(x)).
 * https://oeis.org/A081566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 22, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A081566 – second binomial transform of expansion of exp(3cosh(x)).
   Recurrence (with a(0)=1):
      a(n+1) = Σ_{k=0}^{n} C(n,k) * d_{k+1} * a(n-k)
   where
      d_1   = 2,
      d_i   = 3  if i is even (i ≥ 2),
      d_i   = 0  otherwise.
   This follows directly from the EGF
      A(x) = exp(2x) * exp(3cosh(x))/e^3 = exp(3cosh(x)+2x-3)   (OEIS). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* External types provided by the build system */
typedef uint64_t idx_t;   /* index type   */
typedef seq_t    /* value type, defined elsewhere */  seq_t;

/* The function must have external linkage */
seq_t a(idx_t n)
{
    /* Static cache of computed values */
    static seq_t *values = NULL;
    static size_t cache_cap = 0;    /* current capacity of the cache   */
    static idx_t  computed_max = 0; /* highest index whose value is known */

    /* Ensure the cache can hold indices up to n (inclusive) */
    if ((size_t)(n + 1) > cache_cap) {
        size_t needed = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(values, needed * sizeof(seq_t));
        if (!tmp)  /* allocation failure – abort */
            abort();
        values = tmp;
        /* Zero new entries (not strictly necessary but harmless) */
        for (size_t i = cache_cap; i < needed; ++i)
            values[i] = (seq_t)0;
        cache_cap = needed;
    }

    /* The value a(0) is known from the start */
    if (computed_max == 0 && values[0] == (seq_t)0)
        values[0] = (seq_t)1;

    /* Compute successive terms until we reach the requested index n */
    while (computed_max < n) {
        idx_t m = computed_max;              /* current n in the recurrence   */
        seq_t sum   = (seq_t)0;
        seq_t binom = (seq_t)1;              /* C(m,0) */

        for (idx_t k = 0; k <= m; ++k) {
            /* Determine d_{k+1} */
            seq_t d;
            if (k == 0)                  d = (seq_t)2;           /* i = 1   */
            else if ((k + 1) % 2U == 0U) d = (seq_t)3;           /* even i */
            else                       d = (seq_t)0;             /* odd i >1*/

            if (d != (seq_t)0)
                sum += binom * d * values[m - k];

            /* Prepare binomial coefficient for next k */
            if (k < m) {
                seq_t num = (seq_t)(m - k);
                seq_t den = (seq_t)(k + 1);
                binom = (binom * num) / den;
            }
        }

        values[m + 1] = sum;
        ++computed_max;
    }

    return values[n];
}
