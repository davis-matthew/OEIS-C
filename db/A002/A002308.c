/*
 * A002308  Consecutive quadratic nonresidues mod p.
 * https://oeis.org/A002308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   search -- a(n) = longest run of consecutive quadratic
 *            nonresidues among 1..p-1, p = prime(n) (sieve for prime(n), mark
 *            squares mod p)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 123 terms through n = 9886 (2026-11-01)
 * Helpers:   nth_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/nth_prime_u64.h"

seq_t a(idx_t n) {
    uint64_t p = nth_prime_u64((uint64_t)n);
    unsigned char *res = (unsigned char *)calloc((size_t)p, 1);
    uint64_t i, run = 0, best = 0;
    for (i = 1; i <= p / 2; i++) res[(i * i) % p] = 1;
    for (i = 1; i < p; i++) {
        if (!res[i]) {
            run++;
            if (run > best) best = run;
        } else {
            run = 0;
        }
    }
    free(res);
    return (seq_t)best;
}
