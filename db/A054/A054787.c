/*
 * A054787  Earliest sequence with a(a(n))=7n.
 * https://oeis.org/A054787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nice, nonn
 * Formula:   * a(7n) = 7*a(n) * a(7n+1) = 7n+2 * a(7n+2) = 49n+7 * a(7n+3) =
 *            7n+4 * a(7n+4) = 49n+21 * a(7n+5) = 7n+6 * a(7n+6) = 49n+35
 * Range:     u8 n <= 38, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)          /* base case */
        return (seq_t)0;

    idx_t q = n / 7;     /* integer division */
    idx_t r = n % 7;     /* remainder 0..6 */

    switch (r) {
        case 0: return (seq_t)7 * a(q);                   /* a(7n)  */
        case 1: return (seq_t)7 * (seq_t)q + (seq_t)2;    /* a(7n+1)*/
        case 2: return (seq_t)49 * (seq_t)q + (seq_t)7;   /* a(7n+2)*/
        case 3: return (seq_t)7 * (seq_t)q + (seq_t)4;    /* a(7n+3)*/
        case 4: return (seq_t)49 * (seq_t)q + (seq_t)21;  /* a(7n+4)*/
        case 5: return (seq_t)7 * (seq_t)q + (seq_t)6;    /* a(7n+5)*/
        case 6: return (seq_t)49 * (seq_t)q + (seq_t)35;  /* a(7n+6)*/
        default: return (seq_t)0;                         /* unreachable */
    }
}
