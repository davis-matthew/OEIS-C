/*
 * A089369  Begin with n and place (n-1) on the least significant side, place
 *          (n-2) on the most significant side and so on until a 1 is placed.
 * https://oeis.org/A089369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is formed by starting with n and alternately appending
 *            (n‑1) to the right, (n‑2) to the left, (n‑3) to the right, etc.,
 *            until 1 is placed
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    seq_t res = (seq_t)n;                     /* start with n */

    for (idx_t i = n - 1; i; --i)             /* i = n‑1 … 1 */
    {
        if ((n - i) & 1U)                     /* least significant side */
        {
            int di = digit_len(i);            /* digits of i            */
            seq_t powi = pow10((idx_t)di);    /* 10^digits(i)           */
            res = res * powi + (seq_t)i;      /* append i to the right   */
        }
        else                                  /* most significant side  */
        {
            int d = digit_len(res);           /* digits of current res   */
            seq_t pow = pow10((idx_t)d);      /* 10^digits(res)         */
            res = (seq_t)i * pow + res;       /* prepend i to the left   */
        }
    }

    return res;
}
