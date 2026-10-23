/*
 * A011699  A binary m-sequence: expansion of reciprocal of x^8+x^7+x^5+x+1.
 * https://oeis.org/A011699
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

/* Recurrence: a(n) = a(n-1) XOR a(n-5) XOR a(n-7) XOR a(n-8)
   with initial values: a(0)=0,a(1)=0,a(2)=0,a(3)=0,a(4)=0,a(5)=0,a(6)=0,a(7)=1.
   The sequence is 85‑periodic. */

#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t period[85];
    static int ready = 0;   /* 0 → not computed yet, 1 → ready */

    if (!ready) {
        period[0] = 0;
        period[1] = 0;
        period[2] = 0;
        period[3] = 0;
        period[4] = 0;
        period[5] = 0;
        period[6] = 0;
        period[7] = 1;
        for (idx_t i = 8; i < 85; ++i) {
            period[i] = period[i-1] ^ period[i-5] ^ period[i-7] ^ period[i-8];
        }
        ready = 1;
    }

    return period[(size_t)(n % 85)];
}
