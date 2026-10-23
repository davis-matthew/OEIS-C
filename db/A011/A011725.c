/*
 * A011725  A binary m-sequence: expansion of reciprocal of
 *          x^12+x^7+x^4+x^3+1.
 * https://oeis.org/A011725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-12) XOR a(n-7) XOR a(n-4) XOR a(n-3) for n ≥ 12;
 *            seeds: a(0)=…=a(10)=0, a(11)=1. The sequence is 4095‑periodic
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define PERIOD 4095

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static const size_t period = PERIOD;

    if (!cache) {
        cache = (seq_t *)malloc(period * sizeof(seq_t));
        if (!cache) return 0;  /* allocation failure fallback */

        /* initialize seeds */
        for (size_t i = 0; i < period; ++i)
            cache[i] = 0;
        cache[11] = 1;

        /* recurrence for indices ≥ 12 */
        for (size_t i = 12; i < period; ++i) {
            cache[i] = cache[i-12] ^ cache[i-7] ^ cache[i-4] ^ cache[i-3];
        }
    }

    /* periodicity: a(n + PERIOD) == a(n) */
    idx_t r = n % period;
    return cache[r];
}
