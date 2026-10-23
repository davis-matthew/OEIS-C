/*
 * A036181  Log base 2 (n) mod 101.
 * https://oeis.org/A036181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = discrete logarithm base 2 of n modulo 101. Return the
 *            exponent e such that 2^e ≡ n (mod 101), with 0 ≤ e < 100. For n
 *            ≡ 0 (mod 101) the discrete logarithm is undefined; we return 0
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Precompute the discrete logarithm table for residues 1..100 modulo 101. */
    static seq_t log_tab[101];
    static bool initialized = false;
    if (!initialized) {
        uint32_t v = 1;                /* current residue of 2^e mod 101 */
        for (int e = 0; e < 100; ++e) {
            log_tab[v] = (seq_t)e;    /* store exponent for this residue */
            v = (v * 2u) % 101u;     /* advance to 2^(e+1) */
        }
        initialized = true;
    }

    idx_t r = n % 101ull;           /* reduce n modulo 101 */
    if (r == 0)                     /* undefined; choose 0 as placeholder */
        return 0;
    return log_tab[(idx_t)r];
}
