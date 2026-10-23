/*
 * A184979  n^5 mod 3331.
 * https://oeis.org/A184979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 mod 3331
 * Range:     u8 n <= 3, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n^5 can be reduced modulo 3331;
       we compute (n mod 3331)^5 mod 3331 */
    const uint64_t MOD = 3331ULL;
    uint64_t base = (uint64_t)n % MOD;
    uint64_t res  = 1ULL;
    for (int i = 0; i < 5; ++i)
        res = (res * base) % MOD;
    return (seq_t)res;
}
