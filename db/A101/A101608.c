/*
 * A101608  Solution to Tower of Hanoi puzzle encoded in pairs with the moves
 *          (1,2),(2,3),(3,1),(2,1),(3,2),(1,3). The disks are moved from peg
 *          1 to 2. For a tower of k disks use the first 2^k-1 number pairs.
 * https://oeis.org/A101608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Recurrence: a(1)=1, a(2)=2, a(3)=1, a(4)=3. For n≥5: a(4k+1) =
 *            (k mod 3) + 1, a(4k+2) = ((k+1) mod 3) + 1, a(4k+3) =
 *            f(a(2k+1)), a(4k+4) = f(a(2k+2)), where f(1)=1, f(2)=3, f(3)=2
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   a_rec, f_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a_rec.h"
#include "../utils/f_seq.h"

seq_t a(idx_t n)
{
    return a_rec(n);
}
