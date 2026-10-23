/*
 * A011718  A binary m-sequence: expansion of reciprocal of
 *          x^8+x^6+x^5+x^4+x^3+x+1.
 * https://oeis.org/A011718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A011718: binary m‑sequence from 1/(x⁸+x⁶+x⁵+x⁴+x³+x+1)
 *  Recurrence (mod 2):
 *     a(n) = a(n‑1) ⊕ a(n‑3) ⊕ a(n‑4) ⊕ a(n‑5) ⊕ a(n‑6) ⊕ a(n‑8)
 *     with a(0)=a(1)=…=a(6)=0,  a(7)=1.
 *  The sequence is 85‑periodic.
 */
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t cache[85];
    static bool initialized = false;

    if (!initialized) {
        /* initialise with a(0)..a(9) from the recurrence */
        for (idx_t i = 0; i < 85; ++i)
            cache[i] = 0;
        cache[7] = 1;
        for (idx_t i = 8; i < 85; ++i)
            cache[i] = cache[i-1] ^ cache[i-3] ^ cache[i-4] ^
                       cache[i-5] ^ cache[i-6] ^ cache[i-8];
        initialized = true;
    }

    return cache[n % 85];
}
