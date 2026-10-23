/*
 * A037158  Convolution of natural numbers n >= 1 with Fibonacci numbers F(k),
 *          for k >= -7, with F(-n)=(-1)^(n+1)*F(n).
 * https://oeis.org/A037158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = F(n-3)+(11+8*n)
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute the linear part */
    seq_t term = (seq_t)11 + (seq_t)8 * n;

    /* compute F(n-3) for n>=3, otherwise use negative indices */
    if (n >= 3) {
        idx_t k = n - 3;
        /* iterative Fibonacci for non‑negative k */
        seq_t a0 = 0, a1 = 1;
        if (k == 0)
            return term + a0;
        if (k == 1)
            return term + a1;
        for (idx_t i = 2; i <= k; ++i) {
            seq_t tmp = a0 + a1;
            a0 = a1;
            a1 = tmp;
        }
        return term + a1;
    }

    /* n < 3 : use F(-k) = (-1)^(k+1) * F(k)  with k = 3-n > 0 */
    idx_t pos = 3 - n;                    /* |n-3| */
    seq_t a0 = 0, a1 = 1;
    if (pos == 0)
        return term;                      /* F(0)=0 */
    if (pos == 1)
        a0 = 1;                           /* F(1)=1 */
    else {
        for (idx_t i = 2; i <= pos; ++i) {
            seq_t tmp = a0 + a1;
            a0 = a1;
            a1 = tmp;
        }
    }

    /* sign: negative when (pos+1) is odd */
    if ((pos + 1) & 1U)
        return term - a1;                 /* subtract */
    else
        return term + a1;                 /* add    */
}
