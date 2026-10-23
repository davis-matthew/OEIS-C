/*
 * A101650  A Thue-Morse-Stern sequence.
 * https://oeis.org/A101650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (A002487(n+1) - 1) mod 3 where A002487 is Stern's
 *            diatomic sequence (a(2n) = a(n), a(2n+1) = a(n)+a(n+1), a(0)=0,
 *            a(1)=1)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   stern_mod3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/stern_mod3.h"

seq_t a(idx_t n)
{
    idx_t m = n + 1;                   /* use A002487(n+1) */
    seq_t s = stern_mod3(m);           /* s(n+1) mod 3 */
    return (s + 2) % 3;                /* (−1 + s) mod 3 */
}
