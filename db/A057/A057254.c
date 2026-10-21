/*
 * A057254  Numbers n such that n | 8^n + 7^n + 6^n + 5^n + 4^n.
 * https://oeis.org/A057254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that n divides 8^n + 7^n + 6^n + 5^n + 4^n
   Algorithm: brute‑force search over positive integers,
   testing the congruence with modular exponentiation. */
#include <stdbool.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* offset is 1; for n = 0 the value is irrelevant */

    idx_t found = 0;
    for (seq_t m = 1; ; ++m)
    {
        seq_t s = 0;
        s += mod_pow((seq_t)8, (uint64_t)m, m);
        s += mod_pow((seq_t)7, (uint64_t)m, m);
        s += mod_pow((seq_t)6, (uint64_t)m, m);
        s += mod_pow((seq_t)5, (uint64_t)m, m);
        s += mod_pow((seq_t)4, (uint64_t)m, m);

        if (s % m == 0)
        {
            ++found;
            if (found == n)
                return m;
        }
    }
}
