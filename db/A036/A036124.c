/*
 * A036124  a(n) = 2^n mod 37.
 * https://oeis.org/A036124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n mod 37
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a cached period of length 36, the multiplicative order of 2 modulo 37 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t table[36];
    static bool initialized = false;
    if (!initialized) {
        table[0] = 1;
        for (int i = 1; i < 36; ++i) {
            table[i] = (table[i - 1] * (seq_t)2) % (seq_t)37;
        }
        initialized = true;
    }
    idx_t idx = n % 36;          /* n is uint64_t‑like, 36 fits in idx_t */
    return table[idx];
}
