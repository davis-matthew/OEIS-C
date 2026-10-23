/*
 * A109499  Number of closed walks of length n on the complete graph on 5
 *          nodes from a given node.
 * https://oeis.org/A109499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   a(n) = (4^n + 4*(-1)^n)/5
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses 128‑bit arithmetic for built‑in integral types and
   arbitrary‑precision seq_t for larger types. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Small integral seq_t: use __uint128_t intermediate */
    if (sizeof(seq_t) <= 8) {
        __uint128_t pow4 = 1;
        for (idx_t i = 0; i < n; ++i)
            pow4 *= 4;
        __uint128_t numerator = (n & 1) ? (pow4 - 4) : (pow4 + 4);
        __uint128_t result = numerator / 5;
        return (seq_t)result;
    }
    /* Arbitrary‑precision seq_t: use seq_t arithmetic */
    else {
        seq_t pow4 = 1;       /* 4^n */
        seq_t base = 4;
        idx_t exp = n;
        while (exp) {
            if (exp & 1)
                pow4 *= base;
            base *= base;
            exp >>= 1;
        }
        seq_t numerator = (n & 1) ? (pow4 - (seq_t)4) : (pow4 + (seq_t)4);
        seq_t result = numerator / (seq_t)5;
        return result;
    }
}
