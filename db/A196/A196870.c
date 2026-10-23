/*
 * A196870  a(n+1) = A001610(n)*a(n).
 * https://oeis.org/A196870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prod(F(k)+F(k+2)-1, k=1..n-1), a(1)=1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n <= 1)              /* a(1) = 1, offset = 1 */
        return (seq_t)1;

    seq_t result = 1;

    /* Fibonacci numbers for the current k:
       fkm2 = F(k-1), fkm1 = F(k), fk = F(k+1), fkp1 = F(k+2)   */
    seq_t fkm2 = 0;  /* F(0)   */
    seq_t fkm1 = 1;  /* F(1)   */
    seq_t fk   = 1;  /* F(2)   */
    seq_t fkp1 = 2;  /* F(3)   */

    for (idx_t k = 1; k < n; ++k) {
        seq_t term = fkm1 + fkp1 - 1;
        result *= term;

        /* Advance to next k */
        seq_t next_fkp1 = fkp1 + fk;   /* F(k+3) = F(k+2)+F(k+1) */
        fkm2 = fkm1;
        fkm1 = fk;
        fk   = fkp1;
        fkp1 = next_fkp1;
    }

    return result;
}
