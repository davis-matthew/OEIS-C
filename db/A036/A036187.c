/*
 * A036187  Log base 2 (n) mod 131.
 * https://oeis.org/A036187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = discrete logarithm of n to base 2 modulo 131 (a(n)=0 if n
 *            ≡ 0 (mod 131))
 * Range:     u8 n <= 130, u16 n <= 130, u32 n <= 130, u64 n <= 130,
 *            u128 n <= 130, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t log2table[131];
    static int initialized = 0;

    if (!initialized) {
        /* Build the mapping residue → exponent for 1 ≤ residue ≤ 130.
           For residue 0 (n divisible by 131) we keep a value of 0. */
        idx_t val = 1;           /* 2^0 mod 131 */
        for (idx_t k = 0; k < 130; ++k) {
            log2table[(size_t)val] = (seq_t)k;
            val = (val * 2) % 131;
        }
        log2table[0] = 0;        /* undefined case */
        initialized = 1;
    }

    idx_t r = n % 131;
    return log2table[(size_t)r];
}
