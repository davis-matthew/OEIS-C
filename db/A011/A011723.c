/*
 * A011723  A binary m-sequence: expansion of reciprocal of x^10+x^3+1.
 * https://oeis.org/A011723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-10) XOR a(n-3) for n ≥ 10; a(0..8)=0, a(9)=1
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the binary m‑sequence expansion of 1/(1 + x^3 + x^10) over GF(2). */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t table[1023];        /* 2^10 – 1 = 1023‑periodic sequence */
    if (!initialized) {
        /* seed the first 10 terms */
        for (idx_t i = 0; i < 10; ++i) table[i] = 0;
        table[9] = 1;
        /* fill the rest of the period by the linear recurrence */
        for (idx_t i = 10; i < 1023; ++i) {
            table[i] = table[i-10] ^ table[i-3];
        }
        initialized = true;
    }
    const idx_t PERIOD = 1023;
    idx_t r = n % PERIOD;
    return table[r];
}
