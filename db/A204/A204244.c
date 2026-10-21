/*
 * A204244  Symmetric matrix given by f(i,1)=1, f(1,j)=1, f(i,i)=i! and
 *          f(i,j)=0 otherwise.
 * https://oeis.org/A204244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A204244: a(n) enumerates the symmetric matrix
   f(i,j) defined by f(i,1)=1, f(1,j)=1, f(i,i)=i!, and f(i,j)=0 otherwise.
   The entries are listed in anti‑diagonal order (i+j constant, i increasing). */

#include <stdbool.h>
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset 1, but guard */

    /* Find smallest k such that k*(k+1)/2 >= n.  k = sum-1. */
    idx_t k = 1;
    while ((k * (k + 1) / 2) < n) k++;

    /* Number of terms before sum = k+1 */
    idx_t prev_tri = (k - 1) * k / 2;

    /* Position within the current anti-diagonal (0‑based) */
    idx_t pos = n - prev_tri - 1;

    idx_t i = pos + 1;
    idx_t j = (k + 1) - i;

    if (i == 1 || j == 1)
        return (seq_t)1;
    else if (i == j)
        return factorial(i);
    else
        return (seq_t)0;
}
