/*
 * A037721  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,0,1,3.
 * https://oeis.org/A037721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 2,
   a(n) = 9 * a(n-1) + d_n,
   where d_n is the nth term of the periodic pattern 2,0,1,3 (d_1=2, d_2=0,…). */

#include <stdint.h>   /* for uint64_t if needed */

seq_t a(idx_t n)
{
    /* Handle requests with n == 0 defensively; the OEIS offset is 1. */
    if (n == 0)
        return (seq_t)0;

    seq_t acc = (seq_t)0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t digit;
        /* pattern indices: 0→2, 1→0, 2→1, 3→3 */
        switch ((i - 1) % 4) {
            case 0: digit = (seq_t)2; break;
            case 1: digit = (seq_t)0; break;
            case 2: digit = (seq_t)1; break;
            default: /* case 3 */ digit = (seq_t)3; break;
        }
        acc = acc * (seq_t)9 + digit;
    }
    return acc;
}
