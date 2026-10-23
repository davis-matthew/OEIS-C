/*
 * A037676  Base-6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0,2,3.
 * https://oeis.org/A037676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 6*a(n-1) + d(n), a(0)=0 where d(n) = [1,0,2,3] repeated.
 *            This generates the n‑digit base–6 number whose digits are the
 *            first n terms of the sequence 1,0,2,3,1,0,2,3,…
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t digit;
        switch ((i - 1) % 4) {
            case 0: digit = 1; break;
            case 1: digit = 0; break;
            case 2: digit = 2; break;
            default: digit = 3; break;
        }
        result = result * 6 + digit;
    }
    return result;
}
