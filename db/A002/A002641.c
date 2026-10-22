/*
 * A002641  Numbers k such that (k^2 + k + 1)/7 is prime.
 * https://oeis.org/A002641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search -- a(n) = n-th k with k == 2 or 4 (mod 7) such that
 *            (k^2+k+1)/7 is prime (Miller-Rabin test)
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a002641_isprime, a002641_mulmod, a002641_powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/a002641_isprime.h"
#include "../utils/a002641_mulmod.h"
#include "../utils/a002641_powmod.h"

static uint64_t *a002641_terms = NULL;
static idx_t a002641_cnt = 0;
static idx_t a002641_cap = 0;
static uint64_t a002641_base = 0; /* next block start: candidates base+2, base+4 */

seq_t a(idx_t n)
{
    while (a002641_cnt < n) {
        uint64_t cands[2];
        int j;
        cands[0] = a002641_base + 2;
        cands[1] = a002641_base + 4;
        a002641_base += 7;
        for (j = 0; j < 2; j++) {
            uint64_t k = cands[j];
            uint64_t q = (k * k + k + 1) / 7;
            if (a002641_isprime(q)) {
                if (a002641_cnt == a002641_cap) {
                    idx_t nc = a002641_cap ? a002641_cap * 2 : 1024;
                    a002641_terms = (uint64_t *)realloc(a002641_terms, (size_t)nc * sizeof(uint64_t));
                    a002641_cap = nc;
                }
                a002641_terms[a002641_cnt++] = k;
            }
        }
    }
    return (seq_t)a002641_terms[n - 1];
}
