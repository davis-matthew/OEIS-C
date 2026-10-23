/*
 * A011751  Expansion of (1 + x^4)/(1 + x + x^3 + x^4 + x^5) mod 2.
 * https://oeis.org/A011751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the coefficient of x^n in the series expansion of (1 +
 *            x^4)/(1 + x + x^3 + x^4 + x^5) modulo 2. The sequence is a
 *            linear recurrence of order 5 over GF(2) with period 31: a[n] =
 *            a[n-1] ^ a[n-3] ^ a[n-4] ^ a[n-5] for n ≥ 5 (mod 2)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for bool, true, false */

seq_t a(idx_t n)
{
    static seq_t seq[31];
    static bool initialized = false;
    if (!initialized) {
        seq[0] = 1;
        seq[1] = 1;
        seq[2] = 1;
        seq[3] = 0;
        seq[4] = 1;
        for (idx_t i = 5; i < 31; ++i) {
            seq[i] = seq[i-1] ^ seq[i-3] ^ seq[i-4] ^ seq[i-5];
        }
        initialized = true;
    }
    /* Period is 31, so index modulo 31 gives the correct term */
    return seq[n % 31];
}
