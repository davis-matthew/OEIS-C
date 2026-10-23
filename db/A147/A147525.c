/*
 * A147525  Numbers X such that there exists Y in N : X^2 = 309*Y^2 + 103.
 * https://oeis.org/A147525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 2, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence: A147525 – Numbers X such that X^2 = 309*Y^2 + 103.
 * Recurrence: a(1) = 1818362, a(2) = 233487592671260018
 *              a(n+2) = 128405450990 * a(n+1) - a(n)   for n >= 1
 */
 
#include <stdint.h>
 
seq_t a(idx_t n)
{
    /* If n is less than 1, behaviour is undefined (offset starts at 1). */
    if (n == 1)
        return (seq_t)1818362;
    if (n == 2)
        return (seq_t)233487592671260018;
 
    const seq_t coeff = (seq_t)128405450990;
    seq_t a_prev = (seq_t)1818362;           /* a(1)  */
    seq_t a_curr = (seq_t)233487592671260018;/* a(2)  */
    seq_t a_next;
 
    for (idx_t k = 3; k <= n; ++k)
    {
        a_next = coeff * a_curr - a_prev;
        a_prev = a_curr;
        a_curr = a_next;
    }
 
    return a_curr;
}
