/*
 * A134082  Triangle read by rows, (n-1) zeros followed by (2n, 1).
 * https://oeis.org/A134082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 1325, u16 n <= 1325, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A134082: a(0)=1.  
   For n>0, let r = ceil( (-3+sqrt(9+8n))/2 ).  
   Let p = (r-1)*(r+2)/2 be the number of elements before row r.  
   The row r has (r-1) zeros, then 2r, then 1.  
   Hence a(n)=0 if n-p <= r-1, a(n)=2r if n-p==r, otherwise a(n)=1. */

#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    idx_t nn = n;                       /* index starting at 1 */
    long double disc = sqrtl(9.0L + 8.0L * (long double)nn);
    idx_t s = (idx_t)floorl(disc);
    idx_t r = (s >= 2) ? ((s - 2) / 2) : 0;

    /* adjust r in case floor(sqrt) dropped a bit */
    while ( ((seq_t)r * (r + 3)) / 2 < nn ) ++r;

    seq_t prev = ((seq_t)(r - 1) * (r + 2)) / 2;
    seq_t pos  = (seq_t)nn - prev;          /* position within row r (1‑based) */

    if (pos <= (seq_t)(r - 1))
        return 0;
    if (pos == (seq_t)r)
        return (seq_t)(2 * r);
    return 1;
}
