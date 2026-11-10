/*
 * A060321  Erroneous version of A051694.
 * https://oeis.org/A060321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = A051694(m) = F(z(prime(m))), the smallest Fibonacci
 *            number divisible by prime(m), where z(p) is the Fibonacci rank
 *            of apparition of p and m = n for n <= 13, m = n+1 for n >= 14
 *            (the erroneous entry skips the prime 43). The original erroneous
 *            terms a(6) = 377 (F(14), not 13) and a(13) = 676...
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 15, i64 n <= 15, i128 n <= 15,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 15 (2026-11-07)
 * Helpers:   fibonacci, nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/nth_prime.h"
#include "../utils/fibonacci.h"

static idx_t rank_of_apparition(uint64_t p) {
    uint64_t x = 0, y = 1;
    idx_t k = 1;
    while (y % p != 0) {
        uint64_t t = (x + y) % p;
        x = y;
        y = t;
        k++;
    }
    return k;
}

seq_t a(idx_t n) {
    /* irregular errors present in the dead OEIS entry */
    if (n == 6) return (seq_t)377;
    if (n == 13) return (seq_t)6763;
    uint64_t p = nth_prime(n >= 14 ? n + 1 : n);
    return fibonacci(rank_of_apparition(p));
}
