/*
 * A082489  a(n) = Sum_{k = 0..n} C(n,k) * C(n+k,k) * C(n+2*k,k) * C(n+3*k,k)
 *          * C(n+4*k,k).
 * https://oeis.org/A082489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} C(n,k) * C(n+k,k) * C(n+2*k,k) * C(n+3*k,k)
 *            * C(n+4*k,k)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = binomial(n, k);
        term = term * binomial(n + k, k);
        term = term * binomial(n + 2 * k, k);
        term = term * binomial(n + 3 * k, k);
        term = term * binomial(n + 4 * k, k);
        sum += term;
    }
    return sum;
}
