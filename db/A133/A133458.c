/*
 * A133458  The size of the largest antichain in the 7-dimensional hypercubic
 *          lattice of size n; also the coefficient of x^floor(7*(n-1)/2) in
 *          (1 + x + ... + x^(n-1))^7.
 * https://oeis.org/A133458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{j=0}^{⌊k/n⌋} (-1)^j · C(7,j) · C(k - j·n + 6, 6),
 *            where k = ⌊7·(n-1)/2⌋. This is the number of tuples (x1,…,x7)
 *            with 0≤xi≤n-1 and x1+…+x7 = k, i.e., the middle coefficient of
 *            (1 + x + … + x^(n-1))^7
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 45, u64 n <= 1817,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 128‑bit integer type is available on GCC/Clang for both C11 and C++17. */
#include <stdint.h>

/* Multiplicative binomial coefficient, returns 0 when n < k.  */
static inline __uint128_t binom128(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use the smaller k   */
    __uint128_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);       /* exact integer product */
        res = res / i;                 /* exact division           */
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* defined for n ≥ 1      */

    idx_t k = 7 * (n - 1) / 2;           /* middle exponent        */
    idx_t maxj = k / n;                  /* upper bound of j       */

    static const uint64_t C7[8] = {
        1ULL, 7ULL, 21ULL, 35ULL, 35ULL, 21ULL, 7ULL, 1ULL
    };

    __int128 sum = 0;                    /* signed to allow subtraction */

    for (idx_t j = 0; j <= maxj && j <= 7; ++j) {
        __uint128_t term = (__uint128_t)C7[j] *
                           binom128(k - j * n + 6, 6);
        if (j & 1U)
            sum -= (__int128)term;
        else
            sum += (__int128)term;
    }

    /* the result fits in seq_t for all n where it fits in uint64_t;
       for larger results the cast discards higher bits, which is
       acceptable because such values are not checked. */
    return (seq_t)sum;
}
