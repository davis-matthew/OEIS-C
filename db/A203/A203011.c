/*
 * A203011  (n-1)-st elementary symmetric function of
 *          {1,3,7,15,31,63,...-1+2^n}.
 * https://oeis.org/A203011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=1}^{n} prod_{j!=i} (2^j-1) Recurrence: a(1)=1,
 *            a(n)=P_{n-1} + (2^n-1)*a(n-1), where P_{k} = ∏_{j=1}^{k} (2^j-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset starts at 1, return 0 for safety */
    seq_t prod = 1;            /* P_{i-1} */
    seq_t a_val = 1;          /* a(i-1) */
    seq_t term = 1;           /* 2^i - 1, starts with i=1 */

    if (n == 1) return a_val;

    for (idx_t i = 2; i <= n; ++i) {
        /* next term = 2^i - 1, recurrence: term = term*2 + 1 */
        term = term * 2 + 1;
        /* a(i) = prod + term * a(i-1) */
        seq_t new_val = prod + term * a_val;
        /* update product: prod = prod * term */
        prod = prod * term;
        a_val = new_val;
    }
    return a_val;
}
