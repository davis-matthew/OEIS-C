/*
 * A036188  Log base 3 (n) mod 137.
 * https://oeis.org/A036188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the exponent k (0 ≤ k ≤ 135) with 3^k ≡ n (mod 137); a(n)
 *            = 0 if n is a multiple of 137
 * Range:     u8 n <= 136, u16 n <= 136, u32 n <= 136, u64 n <= 136,
 *            u128 n <= 136, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t log_table[137] = {0};
    static int prepared = 0;

    if (!prepared) {
        seq_t val = 1;                       // 3^0 mod 137
        for (int k = 0; k < 136; ++k) {      // group order is 136
            log_table[(size_t)val] = (seq_t)k;
            val = (val * (seq_t)3) % (seq_t)137;
        }
        prepared = 1;
    }

    idx_t r = n % (idx_t)137;                // residue modulo 137
    if (r == 0) return (seq_t)0;            // 0 for multiples of 137
    return log_table[(size_t)r];
}
