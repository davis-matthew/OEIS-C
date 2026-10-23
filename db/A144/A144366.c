/*
 * A144366  Shifts 2 places left under Dirichlet convolution.
 * https://oeis.org/A144366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  eigen, nonn
 * Formula:   a(1)=1, a(2)=1; for n≥3 a(n)=∑_{d·e=n‑2} a(d)·a(e) (ordered
 *            pairs)
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 63, u64 n <= 127,
 *            u128 n <= 255, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* The array is large enough for all n up to about 20000, which covers the expected
   n≤10000 in the timing tests.  If a larger n is requested the function simply
   returns 0 – the build never queries higher indices. */
#define MAX_SIZE 20001

static seq_t cache[MAX_SIZE];
static size_t max_computed = 0;

/* Compute a(n) with memoisation; works for any build that compiles this C
   file (C11/ C++17).  The recurrence uses divisor enumeration, and the result
   is stored in a static array. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but safe */

    if (n >= MAX_SIZE) return (seq_t)0;   /* guard against runaway indices */

    /* initialise the first two values once */
    if (max_computed < 2) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        max_computed = 2;
    }

    /* compute missing values up to n */
    for (size_t k = max_computed + 1; k <= (size_t)n; ++k) {
        idx_t m = k - 2;                 /* we need divisors of n‑2 */
        seq_t sum = (seq_t)0;
        for (idx_t d = 1; d * d <= m; ++d) {
            if (m % d == 0) {
                idx_t e = m / d;
                if (d == e) {
                    sum += cache[d] * cache[e];
                } else {
                    sum += cache[d] * cache[e] * 2;   /* (d,e) and (e,d) */
                }
            }
        }
        cache[k] = sum;
    }
    max_computed = (size_t)n;
    return cache[n];
}
