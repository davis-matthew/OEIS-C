/*
 * A164692  Number of reduced words of length n in Coxeter group on 47
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^7 = I.
 * https://oeis.org/A164692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=47, a(2)=2162, a(3)=99452, a(4)=4574792,
 * a(5)=210440432, a(6)=9680259872; and for n≥7
 *   a(n)=45*(a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)+a(n-6))
 *          -1035*a(n-7) + (n==7?1:0). */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;
    static idx_t sz   = 0;

    if (n + 1 > sz) {
        idx_t new_sz = n + 1;
        seq_t *tmp   = (seq_t *)realloc(buf, new_sz * sizeof(seq_t));
        buf           = tmp;
        for (idx_t i = sz; i < new_sz; ++i) {
            if (i == 0)
                buf[i] = (seq_t)1;
            else if (i == 1)
                buf[i] = (seq_t)47;
            else if (i == 2)
                buf[i] = (seq_t)2162;
            else if (i == 3)
                buf[i] = (seq_t)99452;
            else if (i == 4)
                buf[i] = (seq_t)4574792;
            else if (i == 5)
                buf[i] = (seq_t)210440432;
            else if (i == 6)
                buf[i] = (seq_t)9680259872;
            else {
                seq_t sum = (seq_t)0;
                sum += (seq_t)45 * buf[i - 1];
                sum += (seq_t)45 * buf[i - 2];
                sum += (seq_t)45 * buf[i - 3];
                sum += (seq_t)45 * buf[i - 4];
                sum += (seq_t)45 * buf[i - 5];
                sum += (seq_t)45 * buf[i - 6];
                sum -= (seq_t)1035 * buf[i - 7];
                if (i == 7)
                    sum += (seq_t)1;
                buf[i] = sum;
            }
        }
        sz = new_sz;
    }
    return buf[n];
}
