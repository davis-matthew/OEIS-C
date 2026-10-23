/*
 * A011708  A binary m-sequence: expansion of reciprocal of
 *          x^8+x^7+x^6+x^3+x^2+x+1.
 * https://oeis.org/A011708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the binary m‑sequence obtained from the recurrence *
 *            a(n) = a(n-1) ⊕ a(n-2) ⊕ a(n-3) ⊕ a(n-6) ⊕ a(n-7) ⊕ a(n-8) *
 *            with initial values a(0)=a(1)=a(2)=a(3)=a(4)=a(5)=a(6)=0,
 *            a(7)=1. * The sequence is 255‑periodic
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
    /* static cache of one full period (255 terms) */
    static seq_t period[255];
    static bool initialized = false;

    /* Initialize the cache on first use */
    if (!initialized) {
        period[0] = period[1] = period[2] = period[3] = period[4] = period[5] = period[6] = 0;
        period[7] = 1;
        for (size_t i = 8; i < 255; ++i) {
            period[i] = period[i-1] ^ period[i-2] ^ period[i-3] ^ period[i-6] ^ period[i-7] ^ period[i-8];
        }
        initialized = true;
    }

    /* Wrap the index to the period length */
    size_t idx = (size_t)(n % 255);
    return period[idx];
}
