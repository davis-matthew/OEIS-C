/*
 * A036206  Log base 3 (n) mod 233.
 * https://oeis.org/A036206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = discrete logarithm base 3 of (n mod 233) in the prime
 *            field modulo 233. If n mod 233 = 0 the value is defined to be 0
 * Range:     u8 n <= 232, u16 n <= 232, u32 n <= 232, u64 n <= 232,
 *            u128 n <= 232, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static table that maps a residue r (1 ≤ r < 233) to the exponent e
       such that 3^e ≡ r (mod 233).  Exponent 0 is associated with r = 1. */
    static bool initialized = false;
    static seq_t log_table[233];

    if (!initialized) {
        /* initialize all entries to 0 */
        for (int i = 0; i < 233; ++i)
            log_table[i] = 0u;
        /* 3^0 = 1 */
        log_table[1] = (seq_t)0u;
        seq_t val = 1u;
        for (idx_t e = 1; e < 233; ++e) {
            val = (val * 3u) % 233u;
            /* the exponent of 1 is already set to 0; subsequent 1 would
               correspond to exponent 233 which we ignore. */
            if (val != 1u)
                log_table[(int)val] = (seq_t)e;
        }
        initialized = true;
    }

    idx_t r = n % 233u;
    if (r == 0u)
        return (seq_t)0u;          /* no discrete log for 0 modulo 233 */
    return log_table[(int)r];
}
