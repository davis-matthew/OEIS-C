/*
 * A221737  Number of 2 X n arrays of occupancy after each element stays put
 *          or moves to some horizontal or antidiagonal neighbor, without
 *          move-in move-out left turns.
 * https://oeis.org/A221737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 15*a(n-1) - 67*a(n-2) + 120*a(n-3) - 80*a(n-4) + 10*a(n-5)
 * for n > 6, with initial terms:
 *   a(1)=1,  a(2)=17,  a(3)=152,  a(4)=1347,  a(5)=11917,  a(6)=105408
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)17;
    if (n == 3) return (seq_t)152;
    if (n == 4) return (seq_t)1347;
    if (n == 5) return (seq_t)11917;
    if (n == 6) return (seq_t)105408;

    /* Use recurrence for n > 6 */
    seq_t w1 = (seq_t)17;      /* a(2) */
    seq_t w2 = (seq_t)152;     /* a(3) */
    seq_t w3 = (seq_t)1347;    /* a(4) */
    seq_t w4 = (seq_t)11917;   /* a(5) */
    seq_t w5 = (seq_t)105408;  /* a(6) */

    for (idx_t i = 7; i <= n; ++i) {
        seq_t next = (seq_t)15 * w5
                   - (seq_t)67 * w4
                   + (seq_t)120 * w3
                   - (seq_t)80 * w2
                   + (seq_t)10 * w1;
        w1 = w2; w2 = w3; w3 = w4; w4 = w5; w5 = next;
    }
    return w5;
}
