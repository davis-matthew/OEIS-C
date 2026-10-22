/*
 * A001136  Primes p such that the multiplicative order of 2 modulo p is
 *          (p-1)/6.
 * https://oeis.org/A001136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- sieve primes p == 1 (mod 6) and keep those where
 *            2^((p-1)/6) == 1 (mod p) and 2^((p-1)/(6q)) != 1 (mod p) for
 *            every prime q dividing (p-1)/6, i.e. ord_p(2) = (p-1)/6
 * Range:     u8 n <= 2, u16 n <= 337, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a1136_order_ok, a1136_powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/a1136_order_ok.h"
#include "../utils/a1136_powmod.h"

static unsigned char *a1136_comp = NULL; /* 1 = composite */
static uint64_t a1136_lim = 0;
static uint64_t *a1136_terms = NULL;
static uint64_t a1136_cnt = 0, a1136_cap = 0;
static uint64_t a1136_next = 7; /* next candidate p (== 1 mod 6) */

static void a1136_sieve(uint64_t lim) {
    uint64_t i, j;
    free(a1136_comp);
    a1136_comp = (unsigned char *)malloc((size_t)(lim + 1));
    memset(a1136_comp, 0, (size_t)(lim + 1));
    a1136_comp[0] = a1136_comp[1] = 1;
    for (i = 2; i * i <= lim; i++)
        if (!a1136_comp[i])
            for (j = i * i; j <= lim; j += i) a1136_comp[j] = 1;
    a1136_lim = lim;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (a1136_lim == 0) a1136_sieve((uint64_t)1 << 16);
    while (a1136_cnt < n) {
        uint64_t p = a1136_next;
        if (p > a1136_lim) {
            a1136_sieve(a1136_lim * 2);
            continue;
        }
        a1136_next += 6;
        if (!a1136_comp[p] && a1136_order_ok(p)) {
            if (a1136_cnt == a1136_cap) {
                uint64_t nc = a1136_cap ? a1136_cap * 2 : 1024;
                uint64_t *nt = (uint64_t *)malloc((size_t)nc * sizeof(uint64_t));
                if (a1136_cnt) memcpy(nt, a1136_terms, (size_t)a1136_cnt * sizeof(uint64_t));
                free(a1136_terms);
                a1136_terms = nt;
                a1136_cap = nc;
            }
            a1136_terms[a1136_cnt++] = p;
        }
    }
    return (seq_t)a1136_terms[n - 1];
}
