/*
 * A011724  A binary m-sequence: expansion of reciprocal of x^11 + x^2 + 1
 *          (mod 2, shifted by 10 initial 0's).
 * https://oeis.org/A011724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-11) (mod 2), a(0)=0, a(1)=0, …, a(9)=0,
 *            a(10)=1
 * Range:     u8 n <= 3000, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t cache[2047];
static bool initialized = false;
static idx_t last_calculated = 0;

seq_t a(idx_t n)
{
    /* Reduce index modulo the period (2047) */
    idx_t m = n % 2047;

    /* Initialise the cache on first call */
    if (!initialized) {
        /* set all to 0 first */
        for (idx_t i = 0; i < 2047; ++i)
            cache[i] = 0;
        cache[10] = 1;              /* a(10) = 1, all other a(0..9) = 0 */
        last_calculated = 10;
        initialized  = true;
    }

    /* If the needed term is already computed, return it */
    if (m <= last_calculated)
        return cache[m];

    /* Compute terms up to index m using the recurrence */
    for (idx_t i = last_calculated + 1; i <= m; ++i) {
        cache[i] = cache[i-2] ^ cache[i-11];
    }
    last_calculated = m;
    return cache[m];
}
