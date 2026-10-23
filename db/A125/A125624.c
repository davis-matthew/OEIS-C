/*
 * A125624  Array read by antidiagonals: n-th row contains the positive
 *          integers with their largest prime factor equal to the n-th prime.
 * https://oeis.org/A125624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = prime(row) * smooth(row,col) where row = k - (n -
 *            T_{k-1}) + 1, col = n - T_{k-1}, T_k = k*(k+1)/2, and
 *            smooth(row,col) is the col‑th integer whose largest prime factor
 *            does not exceed prime(row). The table is read by falling
 *            antidiagonals
 * Range:     u8 n <= 35, u16 n <= 135, u32 n <= 527, u64 n <= 2079,
 *            u128 n <= 5050, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/get_prime.h"

/* Helper: returns the a(n) value for given row (prime index) and
   column (position in that row).  row >= 1, col >= 1. */
static seq_t a_by_row_col(idx_t row, idx_t col)
{
    /* For a given row, all numbers are prime(row) * s,
       where s runs through the s_i-smooth numbers (primes <= prime(row)). */
    /* Precompute the list of primes up to prime(row). */
    uint32_t primes[256];
    for (idx_t pi = 0; pi < row; ++pi) {
        primes[pi] = get_prime(pi + 1);
    }

    /* Generate the first 'col' p_i-smooth numbers. */
    seq_t smooth[256];
    size_t ptr[256] = {0};
    smooth[0] = 1;
    size_t cnt = 1;
    while (cnt < col) {
        seq_t best = 0;
        for (idx_t pi = 0; pi < row; ++pi) {
            seq_t cand = ((seq_t)primes[pi]) * smooth[ptr[pi]];
            if (best == 0 || cand < best) {
                best = cand;
            }
        }
        smooth[cnt++] = best;
        for (idx_t pi = 0; pi < row; ++pi) {
            seq_t cand = ((seq_t)primes[pi]) * smooth[ptr[pi]];
            if (cand == best) {
                ++ptr[pi];
            }
        }
    }

    seq_t result = ((seq_t)get_prime(row)) * smooth[(size_t)(col - 1)];
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Find smallest k such that T_k = k*(k+1)/2 >= n. */
    idx_t k = 1;
    while ((k * (k + 1)) / 2 < n) ++k;

    idx_t prev = ((k - 1) * k) / 2;
    idx_t t = n - prev;                 /* 1 … k   (position inside the diagonal) */
    idx_t row = k - t + 1;              /* row index (prime index)   */
    idx_t col = t;                      /* column index (position)   */

    return a_by_row_col(row, col);
}
