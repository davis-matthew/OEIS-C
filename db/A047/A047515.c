/*
 * A047515  Numbers that are congruent to {0, 3, 4, 6, 7} mod 8.
 * https://oeis.org/A047515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A047515: numbers congruent to {0,3,4,6,7} mod 8
   indexed from 1 (a(1) = 0).  The value at index n is
   a(n) = 8 * floor((n-1)/5) + s[(n-1) % 5] ,
   where s = {0,3,4,6,7}.  The formula works for all n ≥ 1. */

static const seq_t _allowed_mod8[5] = {0, 3, 4, 6, 7};

seq_t a(idx_t n)
{
    /* Convert to zero‑based index.  idx_t is guaranteed to be unsigned. */
    idx_t k = n - 1;

    /* Number of complete 5‑element blocks of the pattern. */
    idx_t block = k / 5;

    /* Position within the current block (0 … 4). */
    idx_t rem   = k % 5;

    /* Compute the result using seq_t arithmetic. */
    return (seq_t)block * 8 + _allowed_mod8[rem];
}
