/*
 * A082298  Expansion of (1-3*x-sqrt(9*x^2-10*x+1))/(2*x).
 * https://oeis.org/A082298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=4,
   (n+1)*a(n) = 5*(2n-1)*a(n‑1) - 9*(n-2)*a(n‑2)  for n>=2 */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    seq_t a_prev2 = (seq_t)1; /* a(0) */
    seq_t a_prev1 = (seq_t)4; /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        if (sizeof(seq_t) == 8) {   /* 64-bit build, use 128-bit intermediate */
            unsigned __int128 num = (unsigned __int128)5 * (2*i - 1) *
                                    (unsigned __int128)a_prev1;
            num -= (unsigned __int128)9 * (i - 2) *
                   (unsigned __int128)a_prev2;
            curr = (seq_t)(num / (i + 1));
        } else {                    /* arbitrary-precision or 128-bit build */
            seq_t num = a_prev1 * (seq_t)(5 * (2*i - 1));
            num = num - a_prev2 * (seq_t)(9 * (i - 2));
            curr = num / (seq_t)(i + 1);
        }

        a_prev2 = a_prev1;
        a_prev1 = curr;
    }

    return a_prev1;
}
