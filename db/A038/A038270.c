/*
 * A038270  Triangle whose (i,j)-th entry is binomial(i,j)*7^(i-j)*4^j.
 * https://oeis.org/A038270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j)*7^(i-j)*4^j
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binomial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find i and j such that n = i*(i+1)/2 + j and 0 ≤ j ≤ i. */

#include <stdint.h>
#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k); */
#include "../utils/pow_seq.h"    /* seq_t pow_seq(seq_t a, int k); */

/* Helper: triangular number t(i) = i*(i+1)/2 using 128‑bit to avoid overflow. */
static inline idx_t triangular(idx_t i)
{
    return (idx_t)((__int128)i * (i + 1) / 2);
}

/* Find the row index i for a given flattened index n. */
static inline idx_t find_row(idx_t n)
{
    idx_t low = 0;
    idx_t high = 1;

    /* Expand high until t(high) > n. */
    while (triangular(high) <= n) {
        low = high;
        high <<= 1;                 /* multiply by 2 */
    }

    /* Binary search between low and high. */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if (triangular(mid) <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Determine row i and column j. */
    idx_t i = find_row(n);
    idx_t j = n - triangular(i);

    /* Compute the term: binomial(i,j) * 7^(i-j) * 4^j. */
    seq_t val = binomial(i, j);

    /* i-j and j should fit in an int for the requested limits. */
    int exp7 = (int)(i - j);
    int exp4 = (int)j;

    val *= pow_seq((seq_t)7, exp7);
    val *= pow_seq((seq_t)4, exp4);

    return val;
}
