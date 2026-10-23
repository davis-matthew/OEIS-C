/*
 * A122742  Numbers of polypentagons with two connected internal vertices (see
 *          Cyvin et al. for precise definition).
 * https://oeis.org/A122742
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 19, u32 n <= 34, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence from the rational generating function: 
   For n ≥ 4
        a(n) = 8*a(n-4) - 8*a(n-3) - 2*a(n-2) + 4*a(n-1) - c_n ,
   where the adjustment c_n is
        c_6 = –1 , c_7 =  1 , c_8 =  4 , c_9 = –6 , c_10 =  2 , otherwise 0.
   Initial terms: a(4)=0, a(5)=0, a(6)=1, a(7)=3, a(8)=6, a(9)=16, a(10)=34. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;          /* off‑offset values are undefined */

    /* Previous four values: a(n‑4), a(n‑3), a(n‑2), a(n‑1) */
    seq_t a_nm4 = 0, a_nm3 = 0, a_nm2 = 0, a_nm1 = 0, a_n = 0;

    for (idx_t k = 4; k <= n; ++k) {
        /* Compute 8*a(n‑4) - 8*a(n‑3) - 2*a(n‑2) + 4*a(n‑1) */
        a_n = a_nm4 * (seq_t)8;   /* 8*a(n‑4) */
        a_n -= a_nm3 * (seq_t)8;  /* -8*a(n‑3) */
        a_n -= a_nm2 * (seq_t)2;  /* -2*a(n‑2) */
        a_n += a_nm1 * (seq_t)4;  /* +4*a(n‑1) */

        /* Apply the specific adjustments for n = 6..10 */
        switch (k) {
            case 6: a_n += (seq_t)1;   break;   /* c_6 = –1  → +1  */
            case 7: a_n -= (seq_t)1;   break;   /* c_7 =  1  → –1  */
            case 8: a_n -= (seq_t)4;   break;   /* c_8 =  4  → –4  */
            case 9: a_n += (seq_t)6;   break;   /* c_9 = –6  → +6  */
            case 10: a_n -= (seq_t)2;  break;   /* c_10 = 2  → –2  */
            default: break;
        }

        /* Shift the history for the next iteration */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
