/*
 * A048494  Array T(k,n) read by antidiagonals: T(n,k) = 2^(n-1) * ((k+1)*n -
 *          2k) + k + 1.
 * https://oeis.org/A048494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(k,n) = 2^(n-1) * ((k+1)*n - 2k) + k + 1 * Sequence a(n) is
 *            read by antidiagonals of the array T(k,n), * starting with n=0.
 *            The mapping from a linear index n to the * indices (k,n) of T
 *            is: * * d = ⌊(√(8n+1)-1)/2⌋ (diagonal number) * t = n - d(d+1)/2
 *            (position inside the diagonal) * k = t (incre...
 * Range:     u8 n <= 27, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/pow2.h"

/* Integer square root of a 64‑bit unsigned integer (floor). */
static idx_t isqrt_u64(idx_t x)
{
    /* Use the double sqrt for an initial approximation – it is exact
       for values up to 2^53, which is more than enough for the loop
       bounds used in this problem.  The subsequent loops clip off
       any rounding error. */
    idx_t r = (idx_t)sqrt((double)x);
    while ((r + 1) * (r + 1) <= x)
        r++;
    while (r * r > x)
        r--;
    return r;
}

seq_t a(idx_t n)
{
    /* The first term is defined to be 1. */
    if (n == 0)
        return (seq_t)1;

    /* Find the diagonal d containing the index n. */
    idx_t sq = isqrt_u64(8 * n + 1);
    idx_t d  = (sq - 1) / 2;

    /* Index of the first element of diagonal d. */
    idx_t tri = d * (d + 1) / 2;

    /* Position inside the diagonal. */
    idx_t t      = n - tri;
    idx_t k     = t;          /* k increases from 0 to d */
    idx_t n_val = d - t;      /* n decreases from d to 0 */

    /* The entry is always 1 when n_val == 0. */
    if (n_val == 0)
        return (seq_t)1;

    /* Compute the term using the closed‑form formula. */
    seq_t base   = pow2(n_val - 1);                     /* 2^(n_val-1) */
    seq_t factor = ((seq_t)(k + 1) * (seq_t)n_val) - (seq_t)(2 * k);
    seq_t val    = base * factor + (seq_t)(k + 1);
    return val;
}
