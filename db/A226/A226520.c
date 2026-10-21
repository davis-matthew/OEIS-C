/*
 * A226520  Irregular triangle read by rows: T(n,k) = Legendre(k,prime(n)),
 *          for 0 <= k < prime(n).
 * https://oeis.org/A226520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabf
 * Formula:   not stated
 * Range:     i8 n <= 4011, i16 n <= 8741, i32 n <= 8741, i64 n <= 8741,
 *            i128 n <= 8741, big any n
 * Helpers:   get_prime, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the A226520 sequence:
   flatten the triangle T(r,k) = Legendre(k, prime(r)) for 0 ≤ k < prime(r).
   The function locates the row containing the n‑th term and returns the
   corresponding Legendre symbol. */

#include <stdint.h>
#include "../utils/get_prime.h"        /* uint32_t get_prime(idx_t k); */
#include "../utils/jacobi_symbol.h"    /* int jacobi_symbol(uint64_t a, uint64_t n); */

static seq_t legendre_symbol(seq_t a, uint32_t p)
{
    /* Handle the case p = 2 explicitly.  For 0≤k<2 only k=0 appears. */
    if (p == 2U) {
        uint64_t r = (uint64_t)(a >= 0 ? a : -a);
        r %= 8ULL;
        return (seq_t)((r == 1ULL || r == 7ULL) ? 1
                                               : (r == 3ULL || r == 5ULL) ? -1
                                                                          : 0);
    }

    /* For odd primes use the Jacobi symbol, which equals the Legendre symbol. */
    uint64_t aa = (uint64_t)(a >= 0 ? a : -a);
    return (seq_t)jacobi_symbol(aa, (uint64_t)p);
}

seq_t a(idx_t n)
{
    /* OEIS offset is 1: a(1) is the first term.  For n==0 return 0. */
    if (n == 0)
        return (seq_t)0;

    idx_t target = n;      /* target position in flattened triangle */
    idx_t cum    = 0;      /* cumulative number of terms up to previous row */
    idx_t row    = 1;      /* current row index (1‑based) */

    /* Find minimal row such that cum + prime(row) >= target */
    while (true) {
        uint32_t p = get_prime(row);
        if (cum + (idx_t)p >= target)
            break;
        cum += (idx_t)p;
        ++row;
    }

    /* Column index within the found row (0‑based) */
    idx_t k = target - cum - 1;
    uint32_t p = get_prime(row);

    return legendre_symbol((seq_t)k, p);
}
