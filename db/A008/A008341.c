/*
 * A008341  a(n)=1, a(n+1) = lcm(a(n),b(n)) / gcd(a(n),b(n)), where {b(n)} =
 *          {fibonacci(n)}.
 * https://oeis.org/A008341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n≥1, a(n+1)=lcm(a(n),Fibonacci(n)) /
 *            gcd(a(n),Fibonacci(n))
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 13, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;             /* not defined for n=0, return 0 */
    seq_t a_prev = 1;                 /* a(1) */
    seq_t f_prev = 1;                 /* Fibonacci(1) */
    seq_t f_next = 1;                 /* Fibonacci(2) */

    for (idx_t i = 1; i < n; ++i) {
        /* Fibonacci(n) needed for a(n+1) */
        seq_t fn = f_prev;

        seq_t g = gcd(a_prev, fn);
        seq_t temp = a_prev / g;
        seq_t a_new = temp * fn;
        a_new = a_new / g;          /* a(n+1) = a(n)*F_n / g^2 */

        a_prev = a_new;

        /* advance Fibonacci */
        seq_t tmp = f_prev + f_next;
        f_prev = f_next;
        f_next = tmp;
    }
    return a_prev;
}
