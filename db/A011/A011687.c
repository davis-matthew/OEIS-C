/*
 * A011687  A binary m-sequence: expansion of reciprocal of x^7 + x^6 + x^5 +
 *          x^4 + 1.
 * https://oeis.org/A011687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n)=a(n-4) ^ a(n-5) ^ a(n-6) ^ a(n-7), with *
 *            a(0)=a(1)=a(2)=a(3)=a(4)=a(5)=0, a(6)=1. This is a *
 *            127‑periodic binary m‑sequence (reciprocal of x^7+x^6+x^5+x^4+1)
 * Range:     u8 n <= 126, u16 n <= 126, u32 n <= 126, u64 n <= 126,
 *            u128 n <= 126, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* 127 terms: indices 0 … 126 */
    static seq_t seq[127];
    static bool ready = false;

    if (!ready) {
        /* initial values */
        seq[0] = 0;
        seq[1] = 0;
        seq[2] = 0;
        seq[3] = 0;
        seq[4] = 0;
        seq[5] = 0;
        seq[6] = 1;

        /* recurrence for n ≥ 7 */
        for (idx_t i = 7; i < 127; ++i) {
            seq[i] = seq[i-4] ^ seq[i-5] ^ seq[i-6] ^ seq[i-7];
        }
        ready = true;
    }

    /* return the term at (n mod 127) */
    return seq[(size_t)(n % 127)];
}
