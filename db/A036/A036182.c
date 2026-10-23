/*
 * A036182  Log base 5 (n) mod 103.
 * https://oeis.org/A036182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = discrete logarithm base 5 of n modulo 103
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* mapping 5^e (mod 103) → e  for 0 ≤ e < 102 */
    static uint8_t log5[103];
    static int initialized = 0;

    if (!initialized) {
        initialized = 1;
        uint32_t val = 1;          /* 5^0 (mod 103) */
        for (uint32_t e = 0; e < 102; ++e) {
            log5[val] = (uint8_t)e;    /* store exponent for residue val */
            val = (val * 5) % 103;     /* next power of 5 */
        }
    }

    uint32_t r = (uint32_t)(n % 103ULL);   /* reduce n modulo 103 */
    if (r == 0)                /* 0 has no discrete log; return 0 by convention */
        return (seq_t)0;

    return (seq_t)log5[r];
}
