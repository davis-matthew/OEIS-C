/*
 * A201685  Triangular array read by rows. T(n,k) is the number of connected
 *          endofunctions on {1,2,...,n} that have exactly k nodes in the
 *          unique cycle of its digraph representation.
 * https://oeis.org/A201685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(r-1,k-1) * r^(r-k) * (k-1)!, where r = smallest
 *            integer with n <= r(r+1)/2, k = n - r(r-1)/2
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 55, u64 n <= 136,
 *            u128 n <= 378, big any n
 * Helpers:   big_pow, binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    idx_t r = 1;
    idx_t t = 1; /* triangular number t_r = r*(r+1)/2 */
    while (t < n) {
        r++;
        t += r;
    }
    idx_t k = n - (t - r);   /* k = n - triangular(r-1) */
    seq_t val = binomial(r - 1, k - 1);
    seq_t power = big_pow((seq_t)r, (seq_t)(r - k));
    val = val * power;
    seq_t fact = factorial(k - 1);
    val = val * fact;
    return val;
}
