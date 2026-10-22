/*
 * A002143  Class numbers h(-p) where p runs through the primes p == 3 (mod
 *          4).
 * https://oeis.org/A002143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = h(-p) where p = A002145(n); for p > 3, h(-p) =
 *            (1/3)*sum(k=1..(p-1)/2, (k|p)) if p == 3 (mod 8), h(-p) =
 *            sum(k=1..(p-1)/2, (k|p)) if p == 7 (mod 8); h(-3) = 1. Quadratic
 *            residues are marked by squaring; p is found by trial division
 * Range:     u8 n <= 1502, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a2143_isprime, a2143_nth_prime_3mod4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/a2143_isprime.h"
#include "../utils/a2143_nth_prime_3mod4.h"

seq_t a(idx_t n) {
    uint64_t p = a2143_nth_prime_3mod4(n);
    if (p == 3) return (seq_t)1;

    unsigned char *isres = (unsigned char *)malloc((size_t)p);
    memset(isres, 0, (size_t)p);
    uint64_t half = (p - 1) / 2;
    for (uint64_t k = 1; k <= half; k++) {
        isres[(k * k) % p] = 1;
    }
    long long s = 0;
    for (uint64_t k = 1; k <= half; k++) {
        s += isres[k] ? 1 : -1;
    }
    free(isres);

    long long h = (p % 8 == 3) ? s / 3 : s;
    return (seq_t)(unsigned long long)h;
}
