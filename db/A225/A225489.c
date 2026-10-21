/*
 * A225489  Elimination order for the first person in a linear Josephus
 *          problem.
 * https://oeis.org/A225489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for OEIS A225489 (Elimination order for the first person in a linear Josephus problem)
 * a(1) = 1
 * For n mod 4 = 0: a(n) = 3*n/4
 * For n mod 4 = 1: a(n) = a(1 + (n-1)/4) + 3*(n-1)/4
 * For n mod 4 = 2: a(n) = a(1 + (n-2)/4) + 3*(n-2)/4 + 1
 * For n mod 4 = 3: a(n) = 3*(n-3)/4 + 2
 */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n <= 1)              /* handles n == 0 (not needed) and n == 1 */
        return (seq_t)n;     /* a(1) = 1, a(0) = 0 if called */

    switch (n % 4)           /* n % 4 is of type idx_t but switch accepts integral constant expression */
    {
        case 0: /* n = 4m */
            return (seq_t)n * 3 / 4;

        case 1: /* n = 4m+1 */
            return a(1 + (n - 1) / 4) + (seq_t)3 * (n - 1) / 4;

        case 2: /* n = 4m+2 */
            return a(1 + (n - 2) / 4) + (seq_t)3 * (n - 2) / 4 + 1;

        case 3: /* n = 4m+3 */
            return (seq_t)3 * (n - 3) / 4 + 2;
    }

    /* unreachable */
    return 0;
}
