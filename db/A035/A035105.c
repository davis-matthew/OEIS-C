/*
 * A035105  a(n) = LCM of Fibonacci sequence {F_1,...,F_n}.
 * https://oeis.org/A035105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = lcm(F_1,F_2,…,F_n) (F_k is the k‑th Fibonacci number)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 12, u64 n <= 16, u128 n <= 24,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 (offset 1).  Handle trivially for safety. */
    if (n == 0) {
        return 0;
    }

    seq_t l = 1;          /* current LCM, starts as lcm(1) = 1        */
    seq_t f_prev = 1;     /* F_{i-1}, initially F_1                 */
    seq_t f_cur  = 1;     /* F_i, initially F_2                     */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t f_i;

        if (i == 1) {
            f_i = 1;              /* F_1 */
        } else if (i == 2) {
            f_i = 1;              /* F_2 */
        } else {
            /* Compute next Fibonacci number: F_{i+1} = F_{i-1} + F_i */
            seq_t f_next = f_prev + f_cur;
            f_prev = f_cur;
            f_cur  = f_next;
            f_i = f_cur;          /* Now f_cur == F_i                         */
        }

        seq_t g = gcd(l, f_i);      /* gcd of current LCM and F_i */
        l = (l / g) * f_i;          /* update LCM */
    }

    return l;
}
