/*
 * A177718  a(n) = |(number of 1's in binary representation of prime(n)) -
 *          (number of 0's in binary representation of prime(n))|.
 * https://oeis.org/A177718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = |A014499(n) - A035103(n)|
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_log2_uint64, get_prime, popcount_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* i.e. the absolute difference between the number of 1‑bits and 0‑bits
   in the binary representation of prime(n).           */

#include "../utils/get_prime.h"         /* uint32_t get_prime(idx_t k);        */
#include "../utils/popcount_u64.h"      /* uint8_t popcount_u64(uint64_t n);  */
#include "../utils/floor_log2_uint64.h"/* idx_t floor_log2_uint64(idx_t v);   */

seq_t a(idx_t n)
{
    /* Get the n‑th prime (1‑based). */
    uint32_t p   = get_prime(n);

    /* Count the 1‑bits in the prime. */
    seq_t ones   = (seq_t)popcount_u64((uint64_t)p);

    /* Compute the bit‑length: floor_log2(p)+1. */
    idx_t bitlen = floor_log2_uint64((idx_t)p) + 1;

    /* Number of 0‑bits in the binary representation. */
    seq_t zeros  = (seq_t)bitlen - ones;

    /* Return the absolute difference. */
    return (ones > zeros) ? (ones - zeros) : (zeros - ones);
}
