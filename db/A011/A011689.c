/*
 * A011689  A binary m-sequence: expansion of reciprocal of x^7+x^3+x^2+x+1.
 * https://oeis.org/A011689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) produced by a binary LFSR with recurrence a(n) = a(n-1) XOR
 *            a(n-2) XOR a(n-3) XOR a(n-7) Initial values: a(0)=…=a(5)=0,
 *            a(6)=1. The sequence is 127‑periodic
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t seq[127];

    if (!initialized) {
        /* zero the state and set the first 1 */
        for (int i = 0; i < 127; ++i) seq[i] = 0;
        seq[6] = (seq_t)1;

        /* generate the full period with the recurrence */
        for (int i = 7; i < 127; ++i) {
            seq[i] = seq[i-1] ^ seq[i-2] ^ seq[i-3] ^ seq[i-7];
        }
        initialized = true;
    }

    return seq[(size_t)(n % 127)];
}
