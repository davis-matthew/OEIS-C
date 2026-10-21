/*
 * A226780  Triangular array read by rows. T(n,k) is the number of 2 tuple
 *          lists of length n that have exactly k coincidences; n >= 0, 0 <= k
 *          <= n.
 * https://oeis.org/A226780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row,k) where row = floor((sqrt(8n+1)-1)/2), k = n -
 *            row(row+1)/2, T(row,k) = binomial(row,k) * A089041(row-k)
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 44, u64 n <= 90, u128 n <= 230,
 *            big any n
 * Helpers:   binomial, factorial_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/factorial_int.h"

/* A089041(m) = Σ_{s=0..m} (-1)^s * binomial(m,s) * factorial(m-s)^2. */
static seq_t A089041(idx_t m) {
    seq_t sum = (seq_t)0;
    for (idx_t s = 0; s <= m; ++s) {
        seq_t term = binomial(m, s);
        seq_t f = factorial_int(m - s);
        term *= f * f;
        if (s & 1U) {
            sum -= term;
        } else {
            sum += term;
        }
    }
    return sum;
}

/* Find the maximal row r such that r(r+1)/2 <= n. */
static idx_t row_of(idx_t n) {
    idx_t r = 0;
    while (((unsigned __int128)(r + 1) * (r + 2)) / 2 <= n) {
        ++r;
    }
    return r;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    idx_t r = row_of(n);
    idx_t k = n - (unsigned __int128)r * (r + 1) / 2;
    seq_t bin = binomial(r, k);
    seq_t val = A089041(r - k);
    return bin * val;
}
