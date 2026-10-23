/*
 * A036208  Log base 7 (n) mod 241.
 * https://oeis.org/A036208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the discrete logarithm of n to base 7 modulo 241, i.e.,
 *            the unique k (0 ≤ k < 240) with 7^k ≡ n (mod 241). For n ≡ 0
 *            (mod 241), a(n) is defined to be 0
 * Range:     u8 n <= 240, u16 n <= 240, u32 n <= 240, u64 n <= 240,
 *            u128 n <= 240, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t log_table[241];
    static int initialized = 0;

    if (!initialized) {
        seq_t pow = 1;                      /* 7^0 mod 241 = 1 */
        for (idx_t k = 0; k < 240; ++k) {
            log_table[(size_t)pow] = (seq_t)k;
            pow = (pow * (seq_t)7) % (seq_t)241;
        }
        initialized = 1;
    }

    idx_t r = n % 241ULL;
    if (r == 0)
        return 0;

    return log_table[(size_t)r];
}
