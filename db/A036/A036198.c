/*
 * A036198  Log base 19 (n) mod 191.
 * https://oeis.org/A036198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = discrete_log_base_19_mod_191(n) that is, a(n) = k such
 *            that 19^k ≡ n (mod 191), 0 ≤ k < 190. If n ≡ 0 (mod 191) the
 *            function returns 0
 * Range:     u8 n <= 190, u16 n <= 190, u32 n <= 190, u64 n <= 190,
 *            u128 n <= 190, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* table[r] gives the exponent k with 19^k ≡ r (mod 191)
       for r = 0..190.  r = 0 is mapped to 0. */
    static seq_t table[191];
    static int initialized = 0;

    if (!initialized) {
        idx_t cur = 1;
        for (idx_t exp = 0; exp < 190; ++exp) {
            table[cur] = (seq_t)exp;
            cur = (cur * 19ULL) % 191ULL;
        }
        table[0] = 0;           /* residue 0 → 0 */
        initialized = 1;
    }

    idx_t r = n % 191ULL;
    return table[r];
}
