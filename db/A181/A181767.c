/*
 * A181767  a(n) = a(n-7) XOR a(n-5) XOR a(n-4) XOR a(n-3) XOR a(n-2) XOR
 *          a(n-1) with initial seven terms 95, 111, 119, 123, 125, 62, and
 *          31.
 * https://oeis.org/A181767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = a(n-7) XOR a(n-5) XOR a(n-4) XOR a(n-3) XOR a(n-2) XOR
 *            a(n-1), a(0)=95, a(1)=111, a(2)=119, a(3)=123, a(4)=125,
 *            a(5)=62, a(6)=31
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* The sequence has period 127, so we precompute the first 127 terms once. */
    static seq_t cache[127];
    static int ready = 0;

    if (!ready) {
        /* initialise the first seven terms */
        cache[0] = 95;
        cache[1] = 111;
        cache[2] = 119;
        cache[3] = 123;
        cache[4] = 125;
        cache[5] = 62;
        cache[6] = 31;

        /* compute terms 7 .. 126 using the recurrence */
        for (int i = 7; i < 127; ++i) {
            cache[i] =
                cache[i-7] ^ cache[i-5] ^ cache[i-4] ^ cache[i-3] ^ cache[i-2] ^ cache[i-1];
        }
        ready = 1;
    }

    /* return the value at index n modulo the period (127) */
    size_t idx = (size_t)(n % 127);
    return cache[idx];
}
