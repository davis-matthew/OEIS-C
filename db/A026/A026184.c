/*
 * A026184  a(n) = (1/3)*s(n), where s(n) is the n-th multiple of 3 in
 *          A026136.
 * https://oeis.org/A026184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(4*n) = 2*n, a(4*n+1) = 6*n + 1, a(4*n+2) = 6*n + 3, a(4*n+3) =
 *            6*n + 5 where n >= 0. Complies with the recurrence a(n) =
 *            2*a(n-4) - a(n-8)
 * Range:     u8 n <= 170, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is 1-based. Compute n mod 4 and use the closed-form patterns. */
    switch (n % 4) {
        case 0: /* n = 4*m,   m = n/4 */
            return (seq_t)2 * (seq_t)(n / 4);
        case 1: /* n = 4*m+1 */
            return (seq_t)6 * (seq_t)((n - 1) / 4) + 1;
        case 2: /* n = 4*m+2 */
            return (seq_t)6 * (seq_t)((n - 2) / 4) + 3;
        case 3: /* n = 4*m+3 */
            return (seq_t)6 * (seq_t)((n - 3) / 4) + 5;
        default: /* unreachable with unsigned n */
            return 0;
    }
}
