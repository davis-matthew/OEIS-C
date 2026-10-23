/*
 * A177056  Decimal expansion of (7/6)^3.
 * https://oeis.org/A177056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(1)=1; a(2)=5; a(3)=8; a(4)=7; for n≥5 the digits repeat as
 *            9,6,2
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* out of range sentinel */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)8;
    if (n == 4) return (seq_t)7;

    idx_t m = n - 5;                     /* position in repeating pattern */
    switch (m % 3) {
        case 0: return (seq_t)9;          /* 9,6,2 repeating */
        case 1: return (seq_t)6;
        case 2: return (seq_t)2;
    }
    return (seq_t)0; /* unreachable, but keeps the compiler happy */
}
