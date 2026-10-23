/*
 * A036175  Log base 7 (n) mod 71.
 * https://oeis.org/A036175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = discrete logarithm base 7 of (n mod 71) in the
 *            multiplicative group modulo 71. 7 is a primitive root modulo 71.
 *            If n ≡ 0 (mod 71) the function returns 0
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t log_table[71];

    if (!initialized) {
        seq_t cur = (seq_t)1;
        for (idx_t k = 0; k < 70; ++k) {
            log_table[(size_t)cur] = (seq_t)k;   // exponent for residue cur
            cur = (cur * (seq_t)7) % (seq_t)71;
        }
        log_table[0] = (seq_t)0;                 // residue 0 is never hit
        initialized = true;
    }

    idx_t m = n % (idx_t)71;
    if (m == 0)
        return (seq_t)0;
    return log_table[(size_t)m];
}
