/*
 * A001914  Cyclic numbers: 10 is a quadratic residue modulo p and class of
 *          mantissa is 2.
 * https://oeis.org/A001914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(1) = 2; for n > 1, a(n) is the (n-1)-th prime p > 5
 *            where the multiplicative order of 10 mod p is exactly (p-1)/2
 * Range:     u8 n <= 16, u16 n <= 1180, u32 n <= 1180, u64 n <= 1180,
 *            u128 n <= 1180, big any n
 * Helpers:   z1914_isprime, z1914_ok, z1914_powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/z1914_isprime.h"
#include "../utils/z1914_ok.h"
#include "../utils/z1914_powmod.h"

static uint64_t *z1914_terms = NULL;
static idx_t z1914_cnt = 0, z1914_cap = 0;
static uint64_t z1914_next = 3;

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)2;
    while (z1914_cnt < n - 1) {
        uint64_t p = z1914_next;
        z1914_next += 2;
        if (z1914_isprime(p) && z1914_ok(p)) {
            if (z1914_cnt == z1914_cap) {
                idx_t nc = z1914_cap ? z1914_cap * 2 : 1024;
                uint64_t *t = (uint64_t *)realloc(z1914_terms, nc * sizeof(uint64_t));
                if (!t) return (seq_t)0;
                z1914_terms = t;
                z1914_cap = nc;
            }
            z1914_terms[z1914_cnt++] = p;
        }
    }
    return (seq_t)z1914_terms[n - 2];
}
