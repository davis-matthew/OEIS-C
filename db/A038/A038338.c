/*
 * A038338  Triangle whose (i,j)-th entry is binomial(i,j)*12^(i-j)*12^j.
 * https://oeis.org/A038338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i, j) * 12^i, where i = ⌊(√(8 n + 1) – 1)/2⌋ and
 *            j = n – i·(i+1)/2
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 37, u64 n <= 123, u128 n <= 444,
 *            big any n
 * Helpers:   binom, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is read row‑by‑row from the triangle T(i,j) =
   binomial(i,j) · 12^(i-j) · 12^j = binomial(i,j) · 12^i. */

/* Uses only integer arithmetic; see the resource functions below. */
#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/seq_pow.h"

/* Compute the row index i given n.  Uses a binary search with 128‑bit
   arithmetic to avoid overflow for large n. */
static idx_t _row_index(idx_t n)
{
    idx_t low = 0;
    idx_t high = 1;
    /* Double high until high*(high+1)/2 > n. */
    while ((__int128)high * (high + 1) / 2 <= n) {
        high <<= 1;
    }
    /* Binary search for the largest i with i*(i+1)/2 <= n. */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((__int128)mid * (mid + 1) / 2 <= n) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    return low;
}

seq_t a(idx_t n)
{
    idx_t i = _row_index(n);          /* row number */
    idx_t j = n - i * (i + 1) / 2;    /* column number */

    seq_t bin = binom(i, j);          /* binomial(i, j) */
    seq_t pow12 = seq_pow((seq_t)12, i); /* 12^i */
    return bin * pow12;
}
