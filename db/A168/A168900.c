/*
 * A168900  Number of reduced words of length n in Coxeter group on 31
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^21 = I.
 * https://oeis.org/A168900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A168900:
    a(0) = 1
    for n > 0:
      a(n) = Σ_{k=1}^{20} 29 * a(n–k)
             – 435 * a(n–21)
             + p(n)
    where
      p(0)   = 1,
      p(1) … p(20) = 2,
      p(21) = 1,
      p(n)  = 0  otherwise.
*/

#include <stddef.h>

seq_t a(idx_t n)
{
    /* 21‑term rolling buffer for the recurrence. */
    seq_t buf[21] = {0};
    buf[0] = (seq_t)1;                 /* a(0) */

    for (idx_t cur = 1; cur <= n; ++cur) {
        seq_t rhs = 0;

        /* Σ_{k=1}^{min(20,cur)} 29*a(cur-k) */
        idx_t maxk = (cur < 20 ? cur : 20);
        for (idx_t k = 1; k <= maxk; ++k) {
            size_t idx = (size_t)((cur - k) % 21);
            rhs += (seq_t)29 * buf[idx];
        }

        /* – 435*a(cur–21)  (if applicable) */
        if (cur >= 21) {
            size_t idx = (size_t)((cur - 21) % 21);
            rhs -= (seq_t)435 * buf[idx];
        }

        /* Add the polynomial term p(cur) */
        seq_t pc = 0;
        if (cur <= 20)
            pc = (seq_t)2;
        else if (cur == 21)
            pc = (seq_t)1;

        seq_t cur_val = rhs + pc;

        /* Store a(cur) in the circular buffer */
        buf[(size_t)(cur % 21)] = cur_val;
    }

    return buf[(size_t)(n % 21)];
}
