/*
 * A002407  Cuban primes: primes which are the difference of two consecutive
 *          cubes.
 * https://oeis.org/A002407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 3*k*(k+1) + 1 for the n-th k >= 1 where that value is
 *            prime (search over k, Miller-Rabin primality test, results
 *            memoized)
 * Range:     u8 n <= 5, u16 n <= 53, u32 n <= 6336, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cuban_isprime, cuban_mulmod, cuban_powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/cuban_isprime.h"
#include "../utils/cuban_mulmod.h"
#include "../utils/cuban_powmod.h"

static uint64_t *cuban_ks = NULL;
static idx_t cuban_cnt = 0;
static idx_t cuban_cap = 0;
static uint64_t cuban_lastk = 0;

seq_t a(idx_t n) {
    while (cuban_cnt < n) {
        uint64_t k = ++cuban_lastk;
        uint64_t v = 3 * k * (k + 1) + 1;
        if (cuban_isprime(v)) {
            if (cuban_cnt == cuban_cap) {
                idx_t nc = cuban_cap ? cuban_cap * 2 : 1024;
                cuban_ks = (uint64_t *)realloc(cuban_ks, (size_t)nc * sizeof(uint64_t));
                cuban_cap = nc;
            }
            cuban_ks[cuban_cnt++] = k;
        }
    }
    {
        seq_t k = (seq_t)cuban_ks[n - 1];
        return (seq_t)3 * k * (k + (seq_t)1) + (seq_t)1;
    }
}
