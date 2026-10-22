/*
 * A001039  a(n) = (p^p-1)/(p-1) where p = prime(n).
 * https://oeis.org/A001039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (p^p-1)/(p-1) = Sum_{i=0..p-1} p^i, where p = prime(n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   nth_prime_a001039
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/nth_prime_a001039.h"

seq_t a(idx_t n) {
    uint64_t p64 = nth_prime_a001039(n);
    seq_t p = (seq_t)p64;
    /* S = 1 + p + ... + p^(k-1), P = p^k; build k = p64 bit by bit */
    seq_t S = 0;
    seq_t P = 1;
    int top = 63;
    while (top > 0 && !((p64 >> top) & 1)) top--;
    int b;
    for (b = top; b >= 0; b--) {
        /* k -> 2k */
        S = S * (P + 1);
        P = P * P;
        if ((p64 >> b) & 1) {
            /* k -> k+1 */
            S = S * p + 1;
            P = P * p;
        }
    }
    return S;
}
