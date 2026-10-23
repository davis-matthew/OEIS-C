/*
 * A028169  Expansion of 1/((1-5*x)*(1-6*x)*(1-7*x)*(1-12*x)).
 * https://oeis.org/A028169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A028169: Expansion of 1/((1-5*x)*(1-6*x)*(1-7*x)*(1-12*x))
   Formula: a(n) = 6^(n+1) - 5^(n+1) + 19*a(n-1) - 84*a(n-2)
   Initial values: a(0)=1, a(1)=30 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)30;

    seq_t a_prevprev = (seq_t)1;      /* a(n-2)   */
    seq_t a_prev     = (seq_t)30;     /* a(n-1)   */

    /* Keep current powers of 5 and 6:
       pow5_current = 5^i,   pow6_current = 6^i   at start of loop  i  */
    seq_t pow5_current = (seq_t)25;    /* 5^2   (i = 2) */
    seq_t pow6_current = (seq_t)36;    /* 6^2   */

    for (idx_t i = 2; i <= n; ++i)
    {
        /* Next powers for 5^(i+1) and 6^(i+1) */
        seq_t pow5_next = pow5_current * (seq_t)5;
        seq_t pow6_next = pow6_current * (seq_t)6;

        /* Recurrence: a(i) = 6^(i+1) - 5^(i+1) + 19*a(i-1) - 84*a(i-2) */
        seq_t term = pow6_next
                     - pow5_next
                     + (seq_t)19 * a_prev
                     - (seq_t)84 * a_prevprev;

        /* Prepare for next iteration */
        a_prevprev = a_prev;
        a_prev     = term;
        pow5_current = pow5_next;
        pow6_current = pow6_next;
    }
    return a_prev;
}
