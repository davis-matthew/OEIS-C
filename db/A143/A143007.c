/*
 * A143007  Square array, read by antidiagonals, where row n equals the
 *          crystal ball sequence for the 2*n-dimensional lattice A_n x A_n.
 * https://oeis.org/A143007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(i,j) with (i,j) obtained from n in antidiagonal order.
 *            T(i,j) = Σ_{k=0..i} binomial(i+k,2k) * binomial(2k,k)^2 *
 *            binomial(j+k,2k)
 * Range:     u8 n <= 22, u16 n <= 48, u32 n <= 141, u64 n <= 446,
 *            u128 n <= 1274, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"

/* Compute T(i,j) as defined above. */
static seq_t compute_T(idx_t i, idx_t j) {
    seq_t sum = 0;
    for (idx_t k = 0; k <= i; ++k) {
        seq_t a = binomial(i + k, 2 * k);
        seq_t b = binomial(2 * k, k);
        seq_t c = binomial(j + k, 2 * k);
        seq_t term = a * b * b * c;
        sum += term;
    }
    return sum;
}

/* Public sequence function. */
seq_t a(idx_t n) {
    /* Find the diagonal s such that total(s-1) <= n < total(s),
       where total(s) = (s+1)(s+2)/2. */
    idx_t cumulative = 0;
    idx_t s = 0;
    while (cumulative + s + 1 <= n) {
        cumulative += s + 1;
        ++s;
    }
    idx_t offset = n - cumulative;
    idx_t i = offset;
    idx_t j = s - offset;
    return compute_T(i, j);
}
