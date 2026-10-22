/*
 * A002384  Numbers m such that m^2 + m + 1 is prime.
 * https://oeis.org/A002384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search: a(n) = n-th m >= 1 such that m^2 + m + 1 is prime
 *            (deterministic Miller-Rabin), memoized
 * Range:     u8 n <= 66, u16 n <= 7364, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a2384_isprime, a2384_mulmod, a2384_powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/a2384_isprime.h"
#include "../utils/a2384_mulmod.h"
#include "../utils/a2384_powmod.h"

static uint64_t *a2384_cache = NULL;
static idx_t a2384_len = 0;
static idx_t a2384_cap = 0;
static uint64_t a2384_last = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0;
    while (a2384_len < n) {
        uint64_t m = a2384_last + 1;
        while (!a2384_isprime(m * m + m + 1)) m++;
        a2384_last = m;
        if (a2384_len == a2384_cap) {
            idx_t nc = a2384_cap ? a2384_cap * 2 : 1024;
            uint64_t *p = (uint64_t *)realloc(a2384_cache, (size_t)nc * sizeof(uint64_t));
            if (!p) return 0;
            a2384_cache = p;
            a2384_cap = nc;
        }
        a2384_cache[a2384_len++] = m;
    }
    return (seq_t)a2384_cache[n - 1];
}
