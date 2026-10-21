/*
 * A066799  Square array read by antidiagonals of eventual period of powers of
 *          k mod n; period of repeating digits of 1/n in base k.
 * https://oeis.org/A066799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, base
 * Formula:   not stated
 * Range:     u8 n <= 9536, u16 n <= 9536, u32 n <= 9536, u64 n <= 9536,
 *            u128 n <= 9536, big any n
 * Helpers:   period_modulo
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A066799: square array read by antidiagonals of eventual periods of powers of k mod n.
 *  a(n) is obtained by first finding the antidiagonal coordinates (row i, column j)
 *  that correspond to index n, and then returning the eventual period of the sequence
 *  k^t (mod i).  The period is computed by iterating the sequence until a value
 *  repeats; the length of the eventual cycle is the answer.
 *
 *  For i = 1 the period is defined to be 1 (the first row of the array consists of 1’s).
 */
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/period_modulo.h"

/* Main sequence generator */
seq_t a(idx_t n)
{
    if (n == 0)  /* index starts at 1; return 0 for safety */
        return (seq_t)0;

    /* find diagonal s so that (s-1)*s/2 >= n  and (s-2)*(s-1)/2 < n   */
    idx_t s = 2;
    while ( ((unsigned __int128)(s - 1) * s) / 2 < n )
        ++s;

    /* previous cumulative count up to diagonal s-1 */
    idx_t prev = ((unsigned __int128)(s - 2) * (s - 1)) / 2;
    idx_t pos = n - prev;            /* 1‑based position inside the diagonal */
    idx_t i = pos;                   /* row index   (n in the formula)  */
    idx_t j = s - pos;               /* column index (k in the formula) */

    if (i == 1)                      /* first row → period always 1 */
        return (seq_t)1;

    /* reduce column modulo row because T(i,k)=T(i,k-i) for k>i */
    j %= i;

    return period_modulo(i, j);
}
