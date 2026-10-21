/*
 * A095249  Reverse concatenation of first n positive integers modulo forward
 *          concatenation of first n positive integers.
 * https://oeis.org/A095249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = A000422(n) mod A007908(n) Concatenate 1..n in reverse
 *            order, then compute the result modulo the forward concatenation
 *            of 1..n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* pre‑compute powers of 10 up to 10^19 (enough for any 64‑bit index) */
    static int init = 0;
    static seq_t pow10[20];
    if (!init) {
        pow10[0] = (seq_t)1;
        for (idx_t i = 1; i < 20; ++i)
            pow10[i] = pow10[i - 1] * (seq_t)10;
        init = 1;
    }

    seq_t forward = (seq_t)0;
    seq_t reverse = (seq_t)0;

    /* forward concatenation 1 .. n */
    for (idx_t i = 1; i <= n; ++i) {
        idx_t len;
        if      (i < 10ULL)                 len = 1;
        else if (i < 100ULL)                len = 2;
        else if (i < 1000ULL)               len = 3;
        else if (i < 10000ULL)              len = 4;
        else if (i < 100000ULL)             len = 5;
        else if (i < 1000000ULL)            len = 6;
        else if (i < 10000000ULL)           len = 7;
        else if (i < 100000000ULL)          len = 8;
        else if (i < 1000000000ULL)         len = 9;
        else if (i < 10000000000ULL)        len = 10;
        else if (i < 100000000000ULL)       len = 11;
        else if (i < 1000000000000ULL)      len = 12;
        else if (i < 10000000000000ULL)     len = 13;
        else if (i < 100000000000000ULL)    len = 14;
        else if (i < 1000000000000000ULL)   len = 15;
        else if (i < 10000000000000000ULL)  len = 16;
        else if (i < 100000000000000000ULL) len = 17;
        else if (i < 1000000000000000000ULL)len = 18;
        else                                len = 19;   /* up to 2^64-1 */

        forward = forward * pow10[len] + (seq_t)i;
    }

    /* reverse concatenation n .. 1 */
    for (idx_t i = n; i != 0; --i) {
        idx_t len;
        if      (i < 10ULL)                 len = 1;
        else if (i < 100ULL)                len = 2;
        else if (i < 1000ULL)               len = 3;
        else if (i < 10000ULL)              len = 4;
        else if (i < 100000ULL)             len = 5;
        else if (i < 1000000ULL)            len = 6;
        else if (i < 10000000ULL)           len = 7;
        else if (i < 100000000ULL)          len = 8;
        else if (i < 1000000000ULL)         len = 9;
        else if (i < 10000000000ULL)        len = 10;
        else if (i < 100000000000ULL)       len = 11;
        else if (i < 1000000000000ULL)      len = 12;
        else if (i < 10000000000000ULL)     len = 13;
        else if (i < 100000000000000ULL)    len = 14;
        else if (i < 1000000000000000ULL)   len = 15;
        else if (i < 10000000000000000ULL)  len = 16;
        else if (i < 100000000000000000ULL) len = 17;
        else if (i < 1000000000000000000ULL)len = 18;
        else                                len = 19;   /* up to 2^64-1 */

        reverse = reverse * pow10[len] + (seq_t)i;
    }

    /* result */
    return reverse % forward;
}
