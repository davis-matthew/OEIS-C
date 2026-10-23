/*
 * A036184  Log base 6 (n) mod 109.
 * https://oeis.org/A036184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = smallest e (0 ≤ e < 108) with 6^e ≡ n (mod 109); a(1) =
 *            0, a(0) = 0. This is the discrete logarithm base 6 modulo 109
 * Range:     u8 n <= 108, u16 n <= 108, u32 n <= 108, u64 n <= 108,
 *            u128 n <= 108, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t table[109];
    static bool init = false;

    if (!init) {
        /* Build exponent table: table[v] = e where 6^e ≡ v (mod 109), 0 ≤ e < 108. */
        idx_t v = 1;   /* 6^0 ≡ 1 */
        for (idx_t e = 0; e < 108; ++e) {
            table[v] = (seq_t)e;
            v = (v * 6) % 109;   /* next residue in 0..108 */
        }
        table[0] = 0;   /* 6^e never hits 0, define a(0) = 0 */
        init = true;
    }

    idx_t r = n % 109;   /* reduce the argument modulo 109 */
    return table[r];
}
