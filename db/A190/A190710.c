/*
 * A190710  [(bn+c)r]-b[nr]-[cr], where (r,b,c)=(sqrt(3),4,3) and [ ]=floor.
 * https://oeis.org/A190710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊(4n+3)√3⌋ - 4⌊n√3⌋ - 5; √3 ≈ 1.732…
 * Range:     u8 n <= 132, u16 n <= 132, u32 n <= 132, u64 n <= 132,
 *            u128 n <= 132, big any n
 * Helpers:   floor_sqrt3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt3.h"

seq_t a(idx_t n)
{
    uint64_t nn      = (uint64_t)n;
    uint64_t term1   = floor_sqrt3(4ULL * nn + 3ULL);  /* ⌊(4n+3)√3⌋ */
    uint64_t term2   = floor_sqrt3(nn);                 /* ⌊n√3⌋     */
    uint64_t result  = term1 - 4ULL * term2 - 5ULL;     /* a(n)       */
    return (seq_t)result;
}
