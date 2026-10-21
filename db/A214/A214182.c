/*
 * A214182  Number of 0..6 colorings of an nX4 array circular in the 4
 *          direction with new values 0..6 introduced in row major order.
 * https://oeis.org/A214182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 902*a(n-1) -142783*a(n-2) +7297504*a(n-3)
 *            -157308287*a(n-4) +1529846974*a(n-5) -7002462289*a(n-6)
 *            +14489273756*a(n-7) -12535263856*a(n-8) +3668758080*a(n-9) with
 *            initial values: a(1)=4, a(2)=353, a(3)=147943, a(4)=96673866,
 *            a(5)=68272896836, a(6)=48807260151073, a(7)=34958661918205401,
 *            a(8...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* short‑term constants (fit in 64‑bit) */
    static const seq_t init_small[7] = {
        4ULL,
        353ULL,
        147943ULL,
        96673866ULL,
        68272896836ULL,
        48807260151073ULL,
        34958661918205401ULL
    };

    /* big constants a(8) and a(9) – created once */
    static seq_t val8 = 0, val9 = 0;
    static int big_init = 0;
    if (!big_init) {
        val8 = seq_from_str("25047104071770605888");
        val9 = seq_from_str("17946563397124532921734");
        big_init = 1;
    }

    if (n <= 7) {
        return init_small[n - 1];
    }
    if (n == 8) {
        return val8;
    }
    if (n == 9) {
        return val9;
    }

    /* recurrence coefficients (int64_t suffices, cast to seq_t at use) */
    static const int64_t coeffs[9] = {
        902LL,
        -142783LL,
        7297504LL,
        -157308287LL,
        1529846974LL,
        -7002462289LL,
        14489273756LL,
        -12535263856LL,
        3668758080LL
    };

    /* recent[0..8] holds a(i-9) ... a(i-1) for the current i */
    seq_t recent[9];
    for (size_t i = 0; i < 7; ++i) {
        recent[i] = init_small[i];
    }
    recent[7] = val8;
    recent[8] = val9;

    for (idx_t i = 10; i <= n; ++i) {
        seq_t sum = 0;
        /* sum coeffs[k] * a(i-k-1)  (k=0 -> a(i-1), k=8 -> a(i-9)) */
        for (int k = 0; k < 9; ++k) {
            sum += (coeffs[k]) * recent[8 - k];
        }
        /* shift recent left by one and append new value */
        for (int j = 0; j < 8; ++j) {
            recent[j] = recent[j + 1];
        }
        recent[8] = sum;
    }
    return recent[8];
}
