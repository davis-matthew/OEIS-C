/*
 * A076507  Three people (P1, P2, P3) are in a circle and are saying Hello to
 *          each other. They start with P2 saying "Hello, Hello". Thereafter
 *          Pn says "Hello" for n times the total number of Hello's so far.
 * https://oeis.org/A076507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 21, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A076507
   a(1) = 2.
   For n > 1:
        let S(n-1) = a(1)+a(2)+...+a(n-1)   (the total number of "Hello"s so far)
        let m(n)   = 3 if n % 3 == 2,
                     1 if n % 3 == 0,
                     2 if n % 3 == 1.
        then  a(n) = S(n-1) * m(n).
   This follows from the rule: each new speaker says a number of "Hello"s
   equal to the current total multiplied by a fixed factor that cycles
   3, 1, 2 (the sequence of multipliers for turn 2,3,4,5,6,…).
*/

#include <stddef.h>   /* for size_t, if needed */

/* The function definition required by the test harness */
seq_t a(idx_t n)
{
    if (n == 0)                /* offset is 1 – values below are irrelevant */
        return (seq_t)0;

    /* first term */
    seq_t a_prev = (seq_t)2;
    if (n == 1)
        return a_prev;

    /* running total of all previous terms */
    seq_t sum = a_prev;

    for (idx_t i = 2; i <= n; ++i) {
        /* multiplier depends on i modulo 3 */
        seq_t mult;
        idx_t mod = i % 3;
        if (mod == 2)
            mult = (seq_t)3;     /* i % 3 == 2 → multiplier 3 */
        else if (mod == 0)
            mult = (seq_t)1;     /* i % 3 == 0 → multiplier 1 */
        else
            mult = (seq_t)2;     /* i % 3 == 1 → multiplier 2 */

        /* compute the current term and update running total */
        seq_t a_next = sum * mult;
        sum = sum + a_next;
        a_prev = a_next;
    }

    return a_prev;
}
