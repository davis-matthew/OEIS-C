/*
 * A063319  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          46 ).
 * https://oeis.org/A063319
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 49, i32 n <= 49, i64 n <= 49, i128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Recurrence for A063319:
   a(0)=a(1)=0,
   a(2)=-1, a(3)=21, a(4)=44, a(5)=66, a(6)=88,
   a(7)=110, a(8)=132, a(9)=152, a(10)=176,
   a(11)=196, a(12)=220, a(13)=242;
   For n ≥ 14: a(n) = a(n-4) + a(n-6) - a(n-10)  */
seq_t a(idx_t n) {
    static seq_t *memo = NULL;
    static idx_t memo_size = 0;

    if (n >= memo_size) {
        size_t new_size = (size_t)(n + 1);                /* allocate up to n */
        memo = (seq_t*)realloc(memo, new_size * sizeof(seq_t));

        /* compute values from the current size up to n */
        for (idx_t i = memo_size; i <= n; ++i) {
            seq_t val = 0;
            if (i >= 2) {
                /* coefficient from the numerator of the generating function */
                seq_t coeff = 0;
                switch (i) {
                    case  2: coeff = -1; break;
                    case  3: coeff = 21; break;
                    case  4: coeff = 44; break;
                    case  5: coeff = 66; break;
                    case  6: coeff = 89; break;
                    case  7: coeff = 89; break;
                    case  8: coeff = 89; break;
                    case  9: coeff = 65; break;
                    case 10: coeff = 44; break;
                    case 11: coeff = 20; break;
                    case 12: coeff = -1; break;
                    case 13: coeff =  1; break;
                    default: coeff =  0; break;
                }
                seq_t a4  = (i >= 4)  ? memo[i-4] : 0;
                seq_t a6  = (i >= 6)  ? memo[i-6] : 0;
                seq_t a10 = (i >= 10) ? memo[i-10] : 0;
                val = a4 + a6 - a10 + coeff;
            }
            memo[i] = val;
        }

        memo_size = n + 1;
    }
    return memo[n];
}
