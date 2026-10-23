/*
 * A036195  Log base 2 (n) mod 173.
 * https://oeis.org/A036195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the discrete logarithm base 2 of n modulo 173 * (i.e.,
 *            the smallest non‑negative integer k such that 2^k ≡ n (mod 173),
 *            * with a(n) = 0 when n ≡ 0 (mod 173))
 * Range:     u8 n <= 172, u16 n <= 172, u32 n <= 172, u64 n <= 172,
 *            u128 n <= 172, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Reduce the argument modulo 173.  For 173 itself or any multiple,
     * the discrete logarithm is defined to be 0. */
    uint64_t residue = n % 173ULL;
    if (residue == 0)
        return 0;

    /* Find the exponent k with 2^k ≡ residue (mod 173).  Since 2 is a
       primitive root modulo the prime 173, such k exists in the range
       [0, 171]; a simple linear search is sufficient. */
    uint64_t val = 1;   /* 2^0 mod 173 */
    seq_t k = 0;        /* exponent counter (stored as seq_t for return type) */

    while (val != residue) {
        val = (val * 2ULL) % 173ULL;
        k++;
    }

    return k;
}
