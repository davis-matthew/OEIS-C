/*
 * A017788  Binomial coefficients C(72,n).
 * https://oeis.org/A017788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = C(72, n) (the binomial coefficient)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 25, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* List of all primes <= 72.  This is static and used for the
   prime‑factorisation of the binomial coefficient. */
static const int _primes[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23,
    29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71
};
static const size_t _prime_cnt = sizeof(_primes) / sizeof(int);

/* Legendre's formula: exponent of prime p in n! */
static idx_t _legendre(idx_t n, int p)
{
    idx_t sum = 0;
    for (idx_t div = p; div <= n; div *= (idx_t)p)
        sum += n / div;
    return sum;
}

/* Return C(72, n).  If n > 72 the coefficient is 0. */
seq_t a(idx_t n)
{
    const idx_t N = 72;

    /* Out of range */
    if (n > N) return (seq_t)0;

    /* Use symmetry: C(72, n) = C(72, 72-n) */
    idx_t k = n;
    if (k > N - k) k = N - k;

    seq_t result = 1;

    /* For every prime p <= N compute its exponent in
       N! - k! - (N-k)! using Legendre's formula, then multiply
       the result by p raised to that exponent.  Since the final
       coefficient fits in seq_t, all intermediate products fit
       as well. */
    for (size_t i = 0; i < _prime_cnt; ++i) {
        int  p = _primes[i];
        idx_t e = _legendre(N, p) - _legendre(k, p) - _legendre(N - k, p);
        for (idx_t j = 0; j < e; ++j)
            result = result * (seq_t)p;
    }

    return result;
}
