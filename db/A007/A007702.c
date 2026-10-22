/*
 * A007702  a(n) = prime(n)*...*prime(m), the least product of consecutive
 *          primes which is nondeficient.
 * https://oeis.org/A007702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k = n..A007684(n)} prime(k); search: multiply
 *            consecutive primes from prime(n) until sigma(P) >= 2P
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   is_prime, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/seq_is_fixed_width.h"

static bool too_wide(uint64_t c) {
    if (seq_is_fixed_width() && sizeof(seq_t) * 8 < 64)
        return (c >> (sizeof(seq_t) * 8)) != 0;
    return false;
}

seq_t a(idx_t n) {
    uint64_t c = 1;
    idx_t count = 0;
    while (count < n) {
        c++;
        if (is_prime(c)) count++;
    }
    if (too_wide(c)) return (seq_t)c;

    seq_t P = (seq_t)c;
    /* D = 2P - sigma(P) > 0 while P is deficient */
    seq_t D = P - 1;
    for (;;) {
        c++;
        while (!is_prime(c)) c++;
        if (too_wide(c)) return P;
        seq_t p = (seq_t)c;
        seq_t newP = P * p;
        if (seq_is_fixed_width() && newP / p != P) return newP;
        seq_t p1 = p + 1;
        seq_t q = P / p1;
        seq_t r = P % p1;
        seq_t f = q * 2;
        if (r * 2 >= p1) f += 1;
        /* new deficit = (p+1)D - 2P, positive iff D > floor(2P/(p+1)) */
        if (!(D > f)) return newP;
        D = p1 * (D - q * 2) - r * 2;
        P = newP;
    }
}
