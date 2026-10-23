/*
 * A036205  Log base 6 (n) mod 229.
 * https://oeis.org/A036205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = discrete logarithm base 6 of n modulo 229
 * Range:     u8 n <= 228, u16 n <= 228, u32 n <= 228, u64 n <= 228,
 *            u128 n <= 228, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
seq_t a(idx_t n)
{
    static seq_t log6[229];
    static bool init = false;
    if (!init) {
        seq_t val = 1;
        /* 6^k mod 229 has period 228.  Map each residue to its discrete log. */
        for (idx_t k = 0; k < 228; ++k) {
            log6[(unsigned)val] = (seq_t)k;
            val = (seq_t)(((unsigned int)val * 6u) % 229u);
        }
        init = true;
    }
    idx_t r = n % 229;
    if (r == 0) return 0;          /* 0 has no discrete logarithm mod 229 */
    return log6[(unsigned)r];
}
