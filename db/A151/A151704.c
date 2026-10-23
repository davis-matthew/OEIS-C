/*
 * A151704  a(0)=1, a(1)=0; a(2^i+j) = 2*a(j) + a(j+1) for 0 <= j < 2^i.
 * https://oeis.org/A151704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=0; For n>1, let i = ⌊log₂ n⌋, power = 2^i, j = n -
 *            power. Then a(n) = 2*a(j) + a(j+1)
 * Range:     u8 n <= 123, u16 n <= 4090, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   msb_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/msb_power.h"

static seq_t a_rec(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)0;
    idx_t power = msb_power(n);
    idx_t j = n - power;
    seq_t aj  = a_rec(j);
    seq_t aj1 = a_rec(j + 1);
    return (seq_t)2 * aj + aj1;
}

seq_t a(idx_t n)
{
    return a_rec(n);
}
