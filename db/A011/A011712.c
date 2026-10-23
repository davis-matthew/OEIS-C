/*
 * A011712  A binary m-sequence: expansion of reciprocal of x^8+x^7+x^5+x^4+1.
 * https://oeis.org/A011712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-8) XOR a(n-7) XOR a(n-5) XOR a(n-4) with initial
 *            values a(0)=a(1)=a(2)=a(3)=a(4)=a(5)=a(6)=0 and a(7)=1. The
 *            sequence has period 51
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* period length */
    const idx_t period = 51U;

    /* static cache for one full period */
    static seq_t cache[51] = {0};
    static bool built = false;

    if (!built) {
        /* first seven terms are already zero-initialised */
        cache[7] = 1;

        /* generate the rest of the period using the linear recurrence */
        /* terms i-4, i-5, i-7, i-8 are defined for i >= 8 */
        for (idx_t i = 8; i < period; ++i) {
            cache[i] = cache[i - 4] ^ cache[i - 5] ^ cache[i - 7] ^ cache[i - 8];
        }

        built = true;
    }

    /* return the term in the period corresponding to n */
    return cache[(size_t)(n % period)];
}
