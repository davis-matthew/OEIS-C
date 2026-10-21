/*
 * A220416  Table T(n,k) = ((n+k-1)*(n+k-2)/2+n)^n, n,k >0 read by
 *          antidiagonals.
 * https://oeis.org/A220416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 27, u64 n <= 65, u128 n <= 170,
 *            big any n
 * Helpers:   big_pow, u128_sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A220416: T(n,k) = ((n+k-1)*(n+k-2)/2 + n)^n, read by antidiagonals.
 *
 * The linear indexing satisfies
 *   a(n) = n^(A002260(n)) = n^(n - t(t+1)/2)  with  t = ⌊(-1 + √(8n-7))/2⌋.
 * This implementation maps the linear index to the pair (i,j) on the
 * antidiagonals and evaluates the formula directly.
 */
#include <stdint.h>
#include <math.h>
#include "../utils/big_pow.h"
#include "../utils/u128_sqrt_floor.h"

seq_t a(idx_t n)
{
    /* n >= 1 is assumed, behaviour for n<1 is undefined */
    /* Compute S = minimal integer such that S*(S-1)/2 >= n */
    unsigned __int128 sq = (unsigned __int128)1 + (unsigned __int128)8 * n;
    unsigned __int128 t   = u128_sqrt_floor(sq);          /* floor sqrt(1+8n) */
    unsigned __int128 S   = (unsigned __int128)1 + t;     /* 1 + floor sqrt */
    S /= 2; /* floor((1+t)/2) */
    /* Fix S to be the minimal satisfying the inequality */
    while (S * (S - 1) / 2 < n) ++S;
    while (S >= 2 && (S - 1) * (S - 2) / 2 >= n) --S;

    /* Compute previous cumulative count */
    unsigned __int128 prev = (S - 1) * (S - 2) / 2;
    idx_t i = (idx_t)(n - prev);   /* row index (positive) */
    idx_t j = (idx_t)(S - i);      /* column index */

    /* Evaluate ((i+j-1)*(i+j-2)/2 + i)^i  in seq_t */
    seq_t base = ((seq_t)(i + j - 1) * (seq_t)(i + j - 2) / 2) + (seq_t)i;
    return big_pow(base, (seq_t)i);
}
