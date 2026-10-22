/*
 * A002648  A variant of the cuban primes: primes p = (x^3 - y^3)/(x - y)
 *          where x = y + 2.
 * https://oeis.org/A002648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*A111051(n)^2 + 1; search over even m for primes 3*m^2 +
 *            1 (Miller-Rabin), memoized
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 3205, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   a2648_isprime, a2648_mulmod, a2648_powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/a2648_isprime.h"
#include "../utils/a2648_mulmod.h"
#include "../utils/a2648_powmod.h"

static uint64_t *a2648_ms = NULL;
static idx_t a2648_cnt = 0, a2648_cap = 0;
static uint64_t a2648_next = 2;

seq_t a(idx_t n) {
    while (a2648_cnt < n) {
        uint64_t m = a2648_next;
        a2648_next += 2;
        if (a2648_isprime(3 * m * m + 1)) {
            if (a2648_cnt == a2648_cap) {
                idx_t nc = a2648_cap ? a2648_cap * 2 : 1024;
                a2648_ms = (uint64_t *)realloc(a2648_ms, nc * sizeof(uint64_t));
                a2648_cap = nc;
            }
            a2648_ms[a2648_cnt++] = m;
        }
    }
    seq_t m = (seq_t)a2648_ms[n - 1];
    return (seq_t)3 * m * m + (seq_t)1;
}
