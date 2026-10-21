/*
 * A204767  Quadruples (a,b,c,d) of the form ( n*(n^3-1), n^3-1, 2*n^3+1,
 *          n*(n^3+2) ).
 * https://oeis.org/A204767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(4k+1)=k*(k^3-1), a(4k+2)=k^3-1, a(4k+3)=2*k^3+1,
 *            a(4k+4)=k*(k^3+2)
 * Range:     u8 n <= 15, u16 n <= 63, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t k = (n - 1) / 4 + 1;        /* quadruple index (1‑based) */
    idx_t r = (n - 1) % 4;            /* element within the quadruple */

    seq_t nk = (seq_t)k;              /* cast the integer part to seq_t */

    /* compute k³ */
    seq_t n3 = nk * nk;
    n3 = n3 * nk;

    switch (r)
    {
        case 0:   /* first element: k*(k³-1) */
            return nk * (n3 - (seq_t)1);

        case 1:   /* second element: k³-1 */
            return n3 - (seq_t)1;

        case 2:   /* third element: 2*k³+1 */
            return (n3 * (seq_t)2) + (seq_t)1;

        case 3:   /* fourth element: k*(k³+2) */
            return nk * (n3 + (seq_t)2);
    }

    return (seq_t)0;   /* unreachable, keeps compiler happy */
}
