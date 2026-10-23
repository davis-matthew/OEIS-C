/*
 * A101598  Triangle read by rows: T(n,k) = number of distinct digits in
 *          decimal representation of binomial(n,k), 0<=k<=n.
 * https://oeis.org/A101598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, base
 * Formula:   a(n) = number of distinct decimal digits in binomial(row, col),
 *            where (row, col) is the position of the n‑th element when the
 *            triangle of distinct‑digit counts of binomial coefficients is
 *            read row by row
 * Range:     u8 n <= 82, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Find the row r and column k that correspond to index n. */
    idx_t r = 0;
    idx_t cum = 0;                   /* number of terms before row r */
    while (cum + r + 1 <= n) {
        cum += r + 1;
        r++;
    }
    idx_t k = n - cum;               /* index within the row */
    if (k > r - k)                   /* use symmetry k <= r/2 */
        k = r - k;

    /* Compute the binomial coefficient C(r, k) using the multiplicative
       formula: result = ∏_{i=1}^{k} (r - k + i) / i. */
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = (seq_t)(r - k + i);
        result = result * num / (seq_t)i;   /* exact division */
    }

    /* Count distinct decimal digits in result. */
    seq_t x = result;
    unsigned mask = 0;
    while (x > 0) {
        unsigned d = (unsigned)(x % (seq_t)10);
        mask |= 1U << d;
        x /= (seq_t)10;
    }
    unsigned cnt = 0;
    while (mask) {
        mask &= mask - 1;
        ++cnt;
    }
    return (seq_t)cnt;
}
