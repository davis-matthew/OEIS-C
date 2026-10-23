/*
 * A192469  Coefficient of x in the reduction by x^2->x+3 of the polynomial
 *          p(n,x)=1+x^n+x^(2n).
 * https://oeis.org/A192469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = b(n) + b(2n)
 * where b(0)=0, b(1)=1 and for n≥2, b(n)=b(n-1)+3*b(n-2).
 * This sequence gives the coefficient of x in the reduction of
 * 1+xⁿ+x²ⁿ modulo x² = x+3.
 */

#include <stdint.h>

#ifndef MAX_K
/* maximum n expected is about 10000, so 2n fits in 20000 */
#define MAX_K 20001
#endif

/* static cache for the b(n) values */
static seq_t b[MAX_K];
static idx_t max_prepared = 1;   /* highest index computed so far */

/* initialise the cache with the base values of the recurrence */
static void init_cache(void)
{
    if (max_prepared > 1) return;          /* already initialised */
    b[0] = (seq_t)0;
    if (MAX_K > 1) {
        b[1] = (seq_t)1;
    }
    max_prepared = 1;
}

/* extend the cache up to at least "up_to" (inclusive) */
static void extend_cache(idx_t up_to)
{
    if (up_to <= max_prepared) return;
    if (up_to >= (idx_t)(MAX_K - 1)) {
        /* Bounds exceeded; this implementation is limited to 2*n < MAX_K */
        /* For the intended use (n ≤ 10000) this condition never triggers. */
        up_to = (idx_t)(MAX_K - 1);
    }
    for (idx_t i = max_prepared + 1; i <= up_to; ++i) {
        /* b[i] = b[i-1] + 3*b[i-2] */
        b[i] = b[i - 1] + (seq_t)3 * b[i - 2];
    }
    max_prepared = up_to;
}

/* public function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)0;   /* sequence is 1‑based; 0 is undefined */

    /* need values up to b(2n) */
    idx_t up_to = n + n;
    init_cache();
    extend_cache(up_to);

    return b[n] + b[up_to];
}
