/*
 * A002382  Numbers of the form (p^2 - 49)/120 where p is prime.
 * https://oeis.org/A002382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p^2 - 49)/120 where p is the n-th prime with p ≡ 2 or 3
 *            (mod 5), p > 5, i.e. p ≡
 *            7,13,17,23,37,43,47,53,67,73,77,83,97,103,107,113 (mod 120);
 *            found by search
 * Range:     u8 n <= 21, u16 n <= 208, u32 n <= 3706, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a002382_isprime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/a002382_isprime.h"

seq_t a(idx_t n) {
    static const uint64_t residues[16] = {7, 13, 17, 23, 37, 43, 47, 53,
                                          67, 73, 77, 83, 97, 103, 107, 113};
    idx_t count = 0;
    uint64_t k = 0;
    for (;;) {
        for (int i = 0; i < 16; i++) {
            uint64_t p = k * 120 + residues[i];
            if (a002382_isprime(p)) {
                count++;
                if (count == n) {
                    seq_t sp = (seq_t)p;
                    return (sp * sp - 49) / 120;
                }
            }
        }
        k++;
    }
}
