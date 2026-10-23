/*
 * A008388  Crystal ball sequence for A_6 lattice.
 * https://oeis.org/A008388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1+7*n*(n+1)*(n^2+n+3)*(11*n^2+11*n+14)/60
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 38, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (sizeof(seq_t) <= sizeof(uint64_t)) {              /* 64‑bit or smaller */
        unsigned __int128 nn = n;                         /* use 128‑bit for safety   */
        unsigned __int128 t = nn * (nn + 1);              /* n*(n+1)                  */
        t *= (nn * nn + nn + 3);                          /* …* (n^2+n+3)             */
        t *= (11 * nn * nn + 11 * nn + 14);               /* …* (11*n^2+11*n+14)      */
        t *= 7;                                           /* multiply by 7            */
        t /= 60;                                          /* divide by 60             */
        return (seq_t)(t + 1);                            /* add 1 and cast to seq_t  */
    } else {                                              /* arbitrary precision     */
        seq_t m = (seq_t)n;
        seq_t t = m * (m + 1);                           /* n*(n+1)                  */
        t *= (m * m + m + 3);                            /* …* (n^2+n+3)             */
        t *= ((seq_t)11 * m * m + (seq_t)11 * m + 14);   /* …* (11*n^2+11*n+14)      */
        t *= 7;                                          /* multiply by 7            */
        t /= 60;                                         /* divide by 60             */
        return t + 1;                                    /* add 1                    */
    }
}
