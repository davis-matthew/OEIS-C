/*
 * A087664  Consider recurrence b(0) = n/4, b(k) = b(k-1)*floor(b(k-1));
 *          sequence gives number of steps to reach an integer, or -1 if no
 *          integer is ever reached.
 * https://oeis.org/A087664
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of steps in the iteration b0 = n/4, bk = bk-1 *
 *            floor(bk-1 ) (a(n) = 0 if n/4 is integer, else minimal k such
 *            that bk is integer, or -1)
 * Range:     u8 n <= 14, u16 n <= 112, u32 n <= 112, u64 n <= 112,
 *            u128 n <= 112, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    if (n < 8) /* offset not defined, return 0 as filler */
        return 0;
    seq_t m = (seq_t)n;            /* current numerator (denominator is 4) */
    int steps = 0;
    /* Loop until the current value becomes an integer or we give up */
    for (;;) {
        /* If numerator mod 4 == 0, m/4 is integer */
        if ((m % 4) == 0)
            return (seq_t)steps;
        /* floor(m/4) for positive integers is just integer division by 4 */
        seq_t f = m / 4;
        /* Next value: m' = m * f  (still over denominator 4) */
        m = m * f;
        steps++;
        /* Safety guard: if it takes too many steps we assume it never reaches an integer */
        if (steps > 10000)
            return (seq_t)-1;
    }
}
