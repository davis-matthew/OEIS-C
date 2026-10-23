/*
 * A169229  Number of reduced words of length n in Coxeter group on 24
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^28 = I.
 * https://oeis.org/A169229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
   a(0)=1, a(1)=24, a(2)=552, a(3)=12696, a(4)=292008,
   a(5)=6716184, a(6)=154472232, a(7)=3552861336,
   a(8)=81715810728, a(9)=1879463646744, a(10)=43227663875112,
   a(11)=994236269127576, a(12)=22867434189934248,
   a(13)=525950986368487704, a(14)=12096872686475217192,
   a(15)=278228071788929995416.
   For n ≥ 0 the sequence satisfies  
        a(n) = 22 * (a(n-1)+…+a(n-27)) – 253 * a(n-28) + R(n) ,
   where R(0)=1, R(1)…R(27)=2, and R(n)=0 for n ≥ 28.
   The definition is derived from the rational generating function
   G(t)= (t^28+2t^27+…+2t+1)/(1–22t–…–22t^27+253t^28). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* circular buffer of the last 28 computed values */
    seq_t buf[28] = {0};

    for (idx_t k = 0; k <= n; ++k) {
        seq_t sum = 0;
        /* sum of the previous up to 27 terms */
        for (idx_t i = 1; i <= 27 && k >= i; ++i) {
            sum += buf[(k - i) % 28];
        }

        seq_t term = 0;
        if (k >= 28) {
            term = (seq_t)253 * buf[(k - 28) % 28];
        }

        seq_t rhs = 0;
        if (k == 0) {
            rhs = 1;
        } else if (k <= 27) {
            rhs = 2;
        }

        seq_t val = sum * (seq_t)22 + rhs;
        if (k >= 28) {
            val -= term;
        }

        buf[k % 28] = val;
    }

    return buf[n % 28];
}
