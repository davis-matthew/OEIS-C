/*
 * A176152  Triangle, read by rows, T(n, k) = 2*binomial(n, k)*binomial(n+1,
 *          k)/(k+1) - (k! - n! + (n-k)!).
 * https://oeis.org/A176152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = value at linear index n of the triangle defined by T(m,
 *            k) = 2*binomial(m, k)*binomial(m+1, k)/(k+1) - (k! - m! +
 *            (m-k)!). Sequence a(n) is the triangle read by rows
 * Range:     u8 n <= 21, u16 n <= 45, u32 n <= 91, u64 n <= 231,
 *            u128 n <= 630, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/binomial.h"

static seq_t factorial(idx_t m) {
    seq_t f = 1;
    for (idx_t i = 2; i <= m; ++i) f *= (seq_t)i;
    return f;
}

seq_t a(idx_t N) {
    /* Determine the row r and column k from the linear index N. */
    double t = sqrt(8.0 * (double)N + 1.0);
    idx_t r = (idx_t)((t - 1.0) / 2.0);
    /* Adjust r so that r*(r+1)/2 > N; then decrement. */
    while (r * (r + 1) / 2 <= N) ++r;
    --r;
    idx_t start = r * (r + 1) / 2;
    idx_t k = N - start;

    /* Compute binomial terms. */
    seq_t b1 = binomial(r, k);
    seq_t b2 = binomial(r + 1, k);
    seq_t first = (seq_t)2 * b1 * b2 / ((seq_t)k + 1);

    /* Compute factorials. */
    seq_t rfact = factorial(r);
    seq_t kfact = factorial(k);
    seq_t rc_fact = factorial(r - k);

    /* Combine to obtain the triangle entry. */
    seq_t result = first + rfact - kfact - rc_fact;
    return result;
}
