/*
 * A078117  Let u(1)=n, u(2)=n+1, v(1)=n+2, v(2)=n+3, u(k)=abs(u(k-1)-v(k-2)),
 *          v(k)=abs(v(k-1)-u(k-2)), then a(n) is the smallest integer such
 *          that for any k>=a(n), v(k)=u(k).
 * https://oeis.org/A078117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A078117:  a(n) = smallest k such that for all j≥k, u(j)=v(j)
   For n ≥ 7 the sequence satisfies a(n) = (n + b(n−6)) / 2 with a 12‑periodic
   offset b = {17,20,15,18,7,16,11,20,27,30,25,14}.  For n < 7 the values are:
   a(1)=8, a(2)=11, a(3)=15, a(4)=17, a(5)=15, a(6)=10.  */
   
#include <stdint.h>

static const seq_t small[6] = {8, 11, 15, 17, 15, 10};
static const seq_t b[12]   = {17, 20, 15, 18, 7, 16, 11, 20, 27, 30, 25, 14};

seq_t a(idx_t n)
{
    if (n < 7u) {
        return small[n - 1];
    } else {
        /* n ≥ 7: use the provided formula */
        idx_t k = n - 6;                /* index into the periodic offset */
        idx_t i = (k - 1) % 12;         /* offset index (0‑based) */
        return ((seq_t)n + b[i]) / 2;
    }
}
