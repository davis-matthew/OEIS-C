/*
 * A075002  a(n) = smallest k such that A007908(k) is divisible by n.
 * https://oeis.org/A075002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, changed
 * Formula:   a(n) = smallest k such that A007908(k) is divisible by n where
 *            A007908(k) = concatenation of 1,2,…,k in decimal
 * Range:     u8 n <= 10, u16 n <= 2254, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper: raise 10 to an integer power, result in seq_t.
   Provided by the build utilities. */
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* The sequence is undefined for n = 0; return 0 in that case. */
    if (n == 0) {
        return 0;
    }

    /* Pre‑compute powers of 10 up to 10^11 once. */
    static seq_t pow10[12];
    static bool init = false;
    if (!init) {
        pow10[0] = 1;
        for (int i = 1; i <= 11; ++i) {
            pow10[i] = pow10[i - 1] * 10;
        }
        init = true;
    }

    seq_t mod_n = (seq_t)n;     /* modulus as seq_t for speed. */
    seq_t mod     = 0;           /* current value of A007908 modulo n. */

    for (idx_t k = 1; ; ++k) {
        /* Compute the number of decimal digits of k. */
        idx_t d;
        if (k < 10)          d = 1;
        else if (k < 100)    d = 2;
        else if (k < 1000)   d = 3;
        else if (k < 10000)  d = 4;
        else if (k < 100000) d = 5;
        else if (k < 1000000)    d = 6;
        else if (k < 10000000)   d = 7;
        else if (k < 100000000)  d = 8;
        else if (k < 1000000000) d = 9;
        else if (k < 10000000000) d = 10;
        else d = 11;          /* sufficient for all practical n tested. */

        /* Update the modulus: concatenate k to the current string. */
        seq_t p10 = pow10[d];                    /* 10^d as seq_t */
        mod = (mod * p10 + (seq_t)k) % mod_n;

        if (mod == 0) {
            return (seq_t)k;
        }
    }
}
