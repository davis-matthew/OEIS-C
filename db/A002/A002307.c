/*
 * A002307  Consecutive quadratic residues mod p: a(n) is the maximal number
 *          of positive reduced quadratic residues which appear consecutively
 *          for n-th prime.
 * https://oeis.org/A002307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   search: p = prime(n); mark the squares i^2 mod p for 1 <= i < p,
 *            then a(n) is the longest run of consecutive marked values in
 *            1..p-1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a2307_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/a2307_prime.h"

seq_t a(idx_t n) {
    uint64_t p = a2307_prime(n);
    bool *is_qr = (bool *)malloc(p * sizeof(bool));
    memset(is_qr, 0, p * sizeof(bool));
    for (uint64_t i = 1; i < p; i++)
        is_qr[(i * i) % p] = true;

    uint64_t best = 0, cur = 0;
    for (uint64_t i = 1; i < p; i++) {
        if (is_qr[i]) {
            cur++;
            if (cur > best) best = cur;
        } else {
            cur = 0;
        }
    }
    free(is_qr);
    return (seq_t)best;
}
