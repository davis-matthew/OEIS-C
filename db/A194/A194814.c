/*
 * A194814  Number of integers k in [1,n] such that {n*r+k*r} > {n*r-k*r},
 *          where { } = fractional part and r=(1+sqrt(5))/2 (the golden
 *          ratio).
 * https://oeis.org/A194814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k∈[1,n] such that floor((n+k)φ) – floor((n−k)φ)
 *            * < floor(2kφ)+1. This is equivalent to testing that *
 *            frac((n+k)φ) > frac((n−k)φ). The fractional parts are avoided by
 *            * using only integer arithmetic with a very accurate rational *
 *            approximation of the golden ratio: * * φ ≈ 17711...
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const unsigned long long PHI_NUM = 17711ULL; /* Numerator of φ approximation */
static const unsigned long long PHI_DEN = 10946ULL; /* Denominator of φ approximation */

/* Integer floor of m*φ using the convergent 17711/10946 */
static inline idx_t floor_phi(idx_t m)
{
    return ((unsigned long long)m * PHI_NUM) / PHI_DEN;
}

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t m   = n - k;                              /* m = n-k  */
        idx_t floor_nk   = floor_phi(m);                /* floor((n-k)φ) */
        idx_t floor_npk  = floor_phi(n + k);            /* floor((n+k)φ) */
        idx_t floor_2k   = floor_phi(2 * k);            /* floor(2kφ)   */

        /* d = floor((n+k)φ) - floor((n-k)φ) */
        idx_t d = floor_npk - floor_nk;

        /* Fail iff d ≥ floor(2kφ) + 1 */
        if (d < floor_2k + 1) {
            ++res;   /* success */
        }
    }
    return res;
}
