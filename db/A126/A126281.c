/*
 * A126281  a(n) is the least m to satisfy the requirements of A052130.
 * https://oeis.org/A126281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  more, nonn
 * Formula:   a(1) = 1, a(n) = ⌊ (n-1) * log(3)/log(3/2) ⌋ for n >= 2
 * Range:     u8 n <= 28, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Pre‑computed constants:
       NUM   = floor( log(3)/log(3/2) * 10^18 )
       DEN   = 10^18                                 */
    static const unsigned long long NUM = 2709511415452443000ULL;
    static const unsigned long long DEN = 1000000000000000000ULL;

    if (n == 1) return 1;

    /* Compute ⌊ (n-1) * NUM / DEN ⌋ using 128‑bit arithmetic
       to avoid overflow.  The product fits in 128 bits because
       NUM < 3·10^18 and n <= a few thousand in practice.       */
    __int128 prod = (__int128)(n - 1) * (__int128)NUM;
    seq_t result = (seq_t)(prod / DEN);
    return result;
}
