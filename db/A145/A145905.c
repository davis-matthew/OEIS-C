/*
 * A145905  Square array read by antidiagonals: Hilbert transform of triangle
 *          A060187.
 * https://oeis.org/A145905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 46, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   antidiagonal_offset, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A145905
 *
 *  Let T(n,k) = (2*k + 1)^n  for n,k ≥ 0.
 *  The sequence is obtained by reading the infinite matrix
 *  T(n,k) along successive antidiagonals.  For an index n
 *  let
 *          d  = ⌊(√(8n+1)-1)/2⌋        (the diagonal index)
 *          j  = n - d(d+1)/2           (the column within that diagonal)
 *          i  = d - j                  (the row within that diagonal)
 *  Then
 *          a(n) = (2·j + 1)^i .
 *
 *  The function implements this directly using integer arithmetic
 *  and the utility seq_pow(base, exp) to do the exponentiation.
 *  It works for any seq_t/idx_t combination supported by the
 *  program (uint8_t – __int128,  or arbitrary‑precision integer).
 */

#include <stdint.h>                 /* for __uint128_t, if available */
#include "../utils/seq_pow.h"        /* seq_pow(seq_t, idx_t) */
#include "../utils/antidiagonal_offset.h"

static inline idx_t antidiagonal_idx(idx_t n)
{
    /* Find the greatest d such that d*(d+1)/2 ≤ n
     * Binary search on [0, n] keeps the range tiny (≤64 iterations).   */
    idx_t low = 0;
    idx_t high = n;                  /* d cannot exceed n */

    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* 1. Identify diagonal d */
    idx_t d = antidiagonal_idx(n);

    /* 2. Compute offset of that diagonal */
    idx_t off = antidiagonal_offset(d);

    /* 3. Coordinates within the diagonal */
    idx_t j = n - off;          /* column index */
    idx_t i = d - j;            /* row index   */

    /* 4. Base of the exponentiation */
    seq_t base = (seq_t)(2 * j + 1);

    /* 5. Compute (2*j+1)^i using the utility seq_pow   */
    return seq_pow(base, i);
}
