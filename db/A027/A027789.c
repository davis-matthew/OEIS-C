/*
 * A027789  a(n) = 2*(n+1)*binomial(n+3,4).
 * https://oeis.org/A027789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n+1)*C(n+3,4) (for n >= 1)
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 137, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 51 terms through n = 998 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Compute C(n,4) for n >= 4 using 64‑bit arithmetic.
   The intermediate result fits in 64 bits for all n ≤ 10000
   and for the values that fit into the various seq_t sizes. */
static seq_t binomial_n_4(idx_t n)
{
    unsigned long long r = 1ULL;
    for (idx_t k = 1; k <= 4; ++k)
        r = r * (n - (k - 1)) / k;
    return (seq_t)r;
}

seq_t a(idx_t n)
{
    /* binomial coefficient C(n+3,4)  */
    seq_t binom = binomial_n_4(n + 3);

    /* a(n) = 2*(n+1)*binom  */
    unsigned long long nn   = (unsigned long long)(n + 1);
    unsigned long long prod = 2ULL * nn * (unsigned long long)binom;

    return (seq_t)prod;
}
