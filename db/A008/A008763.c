/*
 * A008763  Expansion of g.f.: x^4/((1-x)*(1-x^2)^2*(1-x^3)).
 * https://oeis.org/A008763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 1/144 * ( 2*n^3 + 9*n*((-1)^n - 1) - 16*((n mod 3 == 2) -
 *            (n mod 3 == 1)) )
 * Range:     u8 n <= 26, u16 n <= 167, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A008763: expansion of x^4/((1-x)*(1-x^2)^2*(1-x^3)). */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* We perform the calculation in a signed 64‑bit integer.
       For all n needed in the tests (up to ~10 000) the intermediate
       values easily fit in int64_t.  The final result is non‑negative,
       so we cast it to seq_t safely. */
    long long N = (long long)n;          /* promote to signed */
    long long num = 2LL * N * N * N;     /* 2*n^3 */

    /* 9*n*((-1)^n - 1).  (-1)^n - 1 is 0 for even n, -2 for odd n. */
    if (N & 1LL)                        /* odd n */
        num += 9LL * N * (-2LL);        /* subtract 18*n */

    /* -16 * ((n mod 3 == 2) - (n mod 3 == 1)) */
    int d = ((N % 3LL == 2LL) ? 1 : 0) - ((N % 3LL == 1LL) ? 1 : 0);
    num += -16LL * d;                   /* subtract or add 16 */

    return (seq_t)(num / 144LL);
}
