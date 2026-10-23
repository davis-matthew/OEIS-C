/*
 * A171008  Write the n-th prime in binary and change all 0's to 1's and all
 *          1's to 0's.
 * https://oeis.org/A171008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the decimal number whose digits are the bitwise
 *            complement of the binary representation of the n-th prime
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   floor_log2_uint64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    /* index starts at 1 */
    if (n < 1)
        return (seq_t)0;

    uint64_t p = (uint64_t)get_prime(n);   /* the n-th prime */
    if (!p)
        return (seq_t)0;                   /* out of range */

    unsigned bitlen = floor_log2_uint64(p) + 1U;
    seq_t result = 0;

    for (idx_t i = bitlen; i > 0; --i)
    {
        uint64_t bit = (p >> (i - 1)) & 1ULL;
        seq_t inv = (seq_t)(1 - bit);      /* complement bit */
        result = result * 10 + inv;        /* build decimal digit by digit */
    }

    return result;
}
