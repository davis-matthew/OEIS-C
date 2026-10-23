/*
 * A181132  a(0)=0; thereafter a(n) = total number of 0's in binary expansions
 *          of 1, ..., n.
 * https://oeis.org/A181132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) computed via the recurrence a(0) = 0, a(1) = 0, a(2n) =
 *            a(n) + a(n-1) + n, a(2n+1) = 2*a(n) + n This recurrence is
 *            equivalent to counting the total number of 0's in the binary
 *            expansions of 1, …, n
 * Range:     u8 n <= 98, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   compute_a_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/compute_a_rec.h"

seq_t a(idx_t n)
{
    return compute_a_rec(n);
}
