/*
 * A038329  Triangle whose (i,j)-th entry is binomial(i,j)*12^(i-j)*3^j.
 * https://oeis.org/A038329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j) * 12^(i-j) * 3^j where (i,j) are obtained
 *            from the row–major order of the triangular array with entries
 *            T(i,j) = binomial(i,j) * 12^(i-j) * 3^j. For a given n (0‑based)
 *            find the unique i such that i*(i+1)/2 ≤ n < (i+1)*(i+2)/2 Then j
 *            = n - i*(i+1)/2
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 44, u64 n <= 154, u128 n <= 597,
 *            big any n
 * Helpers:   binomial, pow_int, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow_int.h"
#include "../utils/seq_mul.h"

/* Find the row index i for a given n using a binary search.
   Uses 128‑bit arithmetic to avoid overflow. */
static idx_t find_row(idx_t n)
{
    idx_t low = 0;
    idx_t high = 1;
    /* Expand high until the cumulative count exceeds n */
    while (( (__uint128_t)high * (high + 1) / 2 ) <= n)
        high <<= 1;
    /* Binary search between low (exclusive) and high (inclusive) */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (( (__uint128_t)mid * (mid + 1) / 2 ) <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    /* Row and column indices */
    idx_t i = find_row(n);
    idx_t j = n - (idx_t)((__uint128_t)i * (i + 1) / 2);

    /* Compute binomial coefficient */
    seq_t value = binomial(i, j);

    /* Multiply by 12^(i-j) and 3^j */
    value = seq_mul(value, pow_int((seq_t)12, i - j));
    value = seq_mul(value, pow_int((seq_t)3, j));

    return value;
}
