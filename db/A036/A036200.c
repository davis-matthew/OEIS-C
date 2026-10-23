/*
 * A036200  Log base 2 (n) mod 197.
 * https://oeis.org/A036200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the unique exponent e (0 <= e < 196) such that 2^e ≡ n
 *            (mod 197). For n ≡ 0 (mod 197) the value is defined to be 0
 * Range:     u8 n <= 196, u16 n <= 196, u32 n <= 196, u64 n <= 196,
 *            u128 n <= 196, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static unsigned short log_table[197];
    static bool initialized = false;

    if (!initialized) {
        unsigned int val = 1;
        for (unsigned int e = 0; e < 196; ++e) {
            log_table[val] = (unsigned short)e;
            val = (val * 2) % 197;
        }
        initialized = true;
    }

    unsigned int r = (unsigned int)(n % 197);
    if (r == 0)
        return 0;
    return (seq_t)log_table[r];
}
