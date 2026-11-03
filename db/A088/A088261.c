/*
 * A088261  Smallest number prefixed with corresponding term of the sequence
 *          of numbers == 1, 3, 7 or 9 (mod 10), i.e., (1, 3, 7, 9, 11, 13,
 *          17, 19, 21, 23, 27, 29, 31, 33, 37, 39, ...) that yields a prime.
 * https://oeis.org/A088261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is the smallest positive integer D such that the
 *            concatenation D||Pn is prime, where Pn = the nth integer coprime
 *            to 10 (i.e., the nth number ≡1,3,7,9 mod10)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Validated: partial, pass, 105 terms through n = 104 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

static inline uint64_t pow10_int(int e) {
    uint64_t r = 1;
    for (int i = 0; i < e; ++i) r *= 10;
    return r;
}

static inline int num_digits_idx(idx_t x) {
    int d = 0;
    do { d++; x /= 10; } while (x);
    return d;
}

seq_t a(idx_t n) {
    /* Pn = 10 * (n/4) + {1,3,7,9}[n%4] */
    idx_t block = n / 4;
    int rem = (int)(n % 4);
    idx_t Pn = 10 * block;
    switch (rem) {
        case 0: Pn += 1; break;
        case 1: Pn += 3; break;
        case 2: Pn += 7; break;
        case 3: Pn += 9; break;
    }

    int len = num_digits_idx(Pn);
    uint64_t base = pow10_int(len);

    for (uint64_t D = 1;; ++D) {
        /* D*base + Pn is guaranteed to fit in uint64_t for n <= 10000 */
        uint64_t cand = D * base + Pn;
        if (is_prime_u64(cand)) {
            return (seq_t)D;
        }
    }
}
