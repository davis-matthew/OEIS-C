/*
 * A036167  Log base 2 (n) mod 37.
 * https://oeis.org/A036167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the exponent k (0 ≤ k ≤ 35) such that 2^k ≡ n (mod 37);
 *            for n ≡ 0 (mod 37) we define a(n) = 0
 * Range:     u8 n <= 36, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
seq_t a(idx_t n)
{
    /* static table mapping residue r (0..36) to exponent k. */
    static seq_t log_table[37];
    static bool init = false;
    if (!init) {
        init = true;
        /* compute 2^k mod 37 for k = 0..35 */
        seq_t val = 1;
        log_table[1] = 0;                      /* 2^0 ≡ 1 (mod 37)   */
        for (idx_t k = 1; k < 36; ++k) {
            val = (val * 2) % 37;               /* 2^k ≡ val (mod 37) */
            log_table[(unsigned)val] = k;
        }
        log_table[0] = 0;                      /* define a(n)=0 for n≡0 (mod 37) */
    }
    idx_t r = n % 37;            /* r in [0,36] */
    return log_table[(unsigned)r];
}
