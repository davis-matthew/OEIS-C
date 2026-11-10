/*
 * A088263  Diagonal of A088262.
 * https://oeis.org/A088263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n-th prime (in increasing order) ending in k, where k
 *            is the n-th odd number not divisible by 5; computed by search
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-07)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    static const int r[4] = {1, 3, 7, 9};
    idx_t m = n - 1;
    seq_t k = (seq_t)(m / 4) * 10 + (seq_t)r[m % 4];

    seq_t mod = 10;
    seq_t t = k;
    while (t >= 10) {
        t /= 10;
        mod *= 10;
    }

    seq_t c = k;
    idx_t found = 0;
    while (1) {
        if (c >= 2 && is_prime_seq(c)) {
            found++;
            if (found == n) return c;
        }
        c += mod;
    }
}
