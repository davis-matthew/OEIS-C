/*
 * A083218  a(n) = n mod (lpf(n+1)+1), where lpf(n) is the least prime factor
 *          of n (A020639).
 * https://oeis.org/A083218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n mod (lpf(n+1)+1)
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   lpf_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/lpf_uint.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;     /* offset starts at 1, but guard against 0 */
    uint64_t m = (uint64_t)n + 1;
    uint64_t lpf_val = lpf_uint(m);
    uint64_t mod_base = lpf_val + 1;
    return (seq_t)((uint64_t)n % mod_base);
}
