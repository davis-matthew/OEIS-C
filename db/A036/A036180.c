/*
 * A036180  Log base 5 (n) mod 97.
 * https://oeis.org/A036180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = discrete log base 5 of n modulo 97. For n ≡ 0 (mod 97)
 *            the value is defined to be 0
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* static table: log_table[r] = e  such that 5^e ≡ r (mod 97) */
    static seq_t log_table[97];
    static int initialized = 0;

    if (!initialized) {
        /* pre‑compute the table only once; use a small integer type for
           the intermediate modular arithmetic because the modulus is 97. */
        uint64_t p = 1;                     /* 5^0 mod 97 = 1 */
        for (idx_t e = 0; e < 96; ++e) {
            log_table[(size_t)p] = (seq_t)e; /* store exponent */
            p = (p * 5) % 97;               /* next residue */
        }
        log_table[0] = 0;                   /* residue 0 → 0 */
        initialized = 1;
    }

    idx_t r = n % 97;                   /* reduce n modulo 97 */
    if (r == 0)
        return (seq_t)0;                 /* undefined residue → 0 */

    return log_table[(size_t)r];        /* discrete log of r */
}
