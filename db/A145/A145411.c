/*
 * A145411  Number of Hamiltonian cycles in K_6 X P_n.
 * https://oeis.org/A145411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 60 a(2) = 12000 a(3) = 1758360 a(n) = 145*a(n-1) +
 *            516*a(n-2) - 288*a(n-3) for n >= 4
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* return 0 for invalid indices (offset is 1) */
    if (n < 1) return 0;

    /* handle the first three values directly */
    if (n == 1) return (seq_t)60;
    if (n == 2) return (seq_t)12000;
    if (n == 3) return (seq_t)1758360;

    /* iterate the recurrence */
    seq_t a1 = (seq_t)60;          /* a(n-3)   */
    seq_t a2 = (seq_t)12000;       /* a(n-2)   */
    seq_t a3 = (seq_t)1758360;     /* a(n-1)   */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = 145*a(i-1) + 516*a(i-2) - 288*a(i-3) */
        next = (seq_t)( (seq_t)145 * a3
                      + (seq_t)516 * a2
                      - (seq_t)288 * a1 );
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
