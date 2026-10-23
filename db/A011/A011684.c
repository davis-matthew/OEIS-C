/*
 * A011684  A binary m-sequence: expansion of reciprocal of x^7+x^4+x^3+x^2+1.
 * https://oeis.org/A011684
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

/* 
 * Formula: a(n) is the nth coefficient of the formal power series over GF(2) for 1/(1 + x² + x³ + x⁴ + x⁷). 
 * Recurrence: 
 *   a(0)=a(1)=a(2)=a(3)=a(4)=a(5)=0, a(6)=1
 *   for n ≥ 7:  a(n) = a(n-2) XOR a(n-3) XOR a(n-4) XOR a(n-7)
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t seq[127];
    static bool initialized = false;
    if (!initialized) {
        /* initial seeds */
        seq[0] = seq[1] = seq[2] = seq[3] = seq[4] = seq[5] = (seq_t)0;
        seq[6] = (seq_t)1;
        /* generate the rest of the period (period = 127) */
        for (int i = 7; i < 127; ++i) {
            seq[i] = seq[i-2] ^ seq[i-3] ^ seq[i-4] ^ seq[i-7];
        }
        initialized = true;
    }
    return seq[n % 127];
}
