/*
 * A076946  Smallest k such that n*(n+1)*(n+2)...*(n+k) >= n^n.
 * https://oeis.org/A076946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k ≥ 0 such that n·(n+1)·…·(n+k) ≥ nⁿ
 * Range:     u8 n <= 272, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Offset 1) - a(1) = 0 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) = 0 */
    if (n == 1)
        return (seq_t)0;

    /* Use long‑double logs to compare products without overflow. */
    const long double EPS = 1e-12L;           /* tolerance for rounding */

    /* Natural logarithm of n.  n fits in idx_t (≤ 2³² typically). */
    long double log_n   = log((long double)n);     /* ln(n)                  */
    /* Target logarithm: ln(nⁿ) = n·ln(n). */
    long double target  = (long double)n * log_n;
    /* Current logarithm of the product.  Initially the product is n. */
    long double log_prod = log_n;

    /* If already ≥ target, k = 0 (only for n = 1 which is handled). */
    if (log_prod >= target - EPS)
        return (seq_t)0;

    /* Incrementally multiply by (n+1), (n+2), …  */
    idx_t k   = 0;           /* number of additional factors beyond n   */
    idx_t i   = n + 1;       /* next integer to multiply by            */

    while (1) {
        log_prod += log((long double)i);  /* add ln(i) to the running log */
        ++k;
        if (log_prod >= target - EPS)
            return (seq_t)k;
        ++i;
    }
}
