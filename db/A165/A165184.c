/*
 * A165184  Number of reduced words of length n in Coxeter group on 4
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^9 = I.
 * https://oeis.org/A165184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=4, a(2)=12, a(3)=36, a(4)=108, a(5)=324,
   a(6)=972, a(7)=2916, a(8)=8748, a(9)=26238.  For n>=10:
   a(n)=2*a(n-1)+2*a(n-2)+2*a(n-3)+2*a(n-4)+2*a(n-5)+2*a(n-6)+2*a(n-7)+2*a(n-8)-3*a(n-9). */
#include <stdint.h>
seq_t a(idx_t n)
{
    /* Initial values for n < 10 */
    static const seq_t init[10] = {
        (seq_t)1,
        (seq_t)4,
        (seq_t)12,
        (seq_t)36,
        (seq_t)108,
        (seq_t)324,
        (seq_t)972,
        (seq_t)2916,
        (seq_t)8748,
        (seq_t)26238
    };
    if (n < 10)
        return init[n];

    /* Circular buffer holding the last 10 computed values (indices modulo 10). */
    seq_t buf[10];
    idx_t i;
    for (i = 0; i < 10; ++i)
        buf[i] = init[i];

    /* Compute values from n = 10 up to the requested n */
    for (i = 10; i <= n; ++i) {
        seq_t next = 2 * buf[(i - 1) % 10];
        next += 2 * buf[(i - 2) % 10];
        next += 2 * buf[(i - 3) % 10];
        next += 2 * buf[(i - 4) % 10];
        next += 2 * buf[(i - 5) % 10];
        next += 2 * buf[(i - 6) % 10];
        next += 2 * buf[(i - 7) % 10];
        next += 2 * buf[(i - 8) % 10];
        next -= 3 * buf[(i - 9) % 10];
        buf[i % 10] = next;
    }
    return buf[n % 10];
}
