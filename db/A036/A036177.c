/*
 * A036177  Log base 3 (n) mod 79.
 * https://oeis.org/A036177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = discrete logarithm of n base 3 modulo 79 (i.e., k with
 *            3^k ≡ n (mod 79), 0 ≤ k < 79; if n ≡ 0 (mod 79) return 0)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* static look‑up table for the discrete log of residues modulo 79 */
    static int exp_map[79];
    static int initialized = 0;

    if (!initialized) {
        /* initialize table: exp_map[r] = e such that 3^e ≡ r (mod 79) */
        for (int i = 0; i < 79; ++i) exp_map[i] = -1;
        int pow = 1;
        for (int e = 0; e < 78; ++e) {
            exp_map[pow] = e;
            pow = (pow * 3) % 79;
        }
        exp_map[0] = 0;          /* residue 0 has no discrete log; use 0 */
        initialized = 1;
    }

    /* reduce n modulo 79 and look up the exponent */
    size_t r = (size_t)(n % 79);
    return (seq_t)exp_map[r];
}
