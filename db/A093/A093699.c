/*
 * A093699  Maximum number of odd 2 X 2 submatrices over all 2n X 2n (0,1)
 *          matrices.
 * https://oeis.org/A093699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 a(n‑1) + 2 a(n‑2) – 6 a(n‑3) + 6 a(n‑5) – 2 a(n‑6) – 2
 *            a(n‑7) + a(n‑8), with a(1)=1, a(2)=24, a(3)=129, a(4)=448,
 *            a(5)=1105, a(6)=2376, a(7)=4417, a(8)=7680. The recurrence comes
 *            from the generating function
 *            x(x⁶+38x⁵+95x⁴+148x³+79x²+22x+1)/[(1‑x)⁵(1+x)³]
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)                     /* for indices below the offset */
        return 0;

    /* static buffer to hold computed values */
    static seq_t *buf = NULL;       /* buf[0] is unused */
    static size_t buf_sz = 0;       /* number of allocated slots */

    /* grow buffer if needed */
    if ((size_t)n >= buf_sz) {
        size_t old_sz = buf_sz;
        size_t new_sz = (size_t)n + 1; /* need indices 0 .. n */
        seq_t *tmp = (seq_t *)realloc(buf, new_sz * sizeof(seq_t));
        if (!tmp)               /* allocation failed */
            return 0;
        buf = tmp;
        buf_sz = new_sz;

        /* initialise new entries */
        for (size_t i = old_sz; i < new_sz; ++i) {
            if (i == 0) {
                buf[0] = 0;
                continue;
            }
            switch ((int)i) { /* base values for i = 1..8 */
                case 1: buf[1] = (seq_t)1;     break;
                case 2: buf[2] = (seq_t)24;    break;
                case 3: buf[3] = (seq_t)129;   break;
                case 4: buf[4] = (seq_t)448;   break;
                case 5: buf[5] = (seq_t)1105;  break;
                case 6: buf[6] = (seq_t)2376;  break;
                case 7: buf[7] = (seq_t)4417;  break;
                case 8: buf[8] = (seq_t)7680;  break;
                default:
                    /* recurrence for n >= 9: */
/*   a_n = 2 a_{n-1} + 2 a_{n-2} - 6 a_{n-3}
          + 6 a_{n-5} - 2 a_{n-6} - 2 a_{n-7} + a_{n-8}  */
                    {
                        seq_t pos = buf[i-1] * 2       /* 2 a_{n-1} */
                                  + buf[i-2] * 2       /* 2 a_{n-2} */
                                  + buf[i-5] * 6       /* 6 a_{n-5} */
                                  + buf[i-8];          /* a_{n-8}   */
                        seq_t neg = buf[i-3] * 6       /* 6 a_{n-3} */
                                  + buf[i-6] * 2       /* 2 a_{n-6} */
                                  + buf[i-7] * 2;      /* 2 a_{n-7} */
                        buf[i] = pos - neg;           /* pos ≥ neg */
                    }
            }
        }
    }
    return buf[n];
}
