/*
 * A002643  Numbers k such that (k^2 + k + 1)/19 is prime.
 * https://oeis.org/A002643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search: a(n) = n-th k >= 1 with k^2+k+1 divisible by 19 (k == 7
 *            or 11 mod 19) and (k^2+k+1)/19 prime (Miller-Rabin)
 * Range:     u8 n <= 12, u16 n <= 992, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a2643_isprime, a2643_mulmod, a2643_powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/a2643_isprime.h"
#include "../utils/a2643_mulmod.h"
#include "../utils/a2643_powmod.h"

static uint64_t *a2643_terms = NULL;
static idx_t a2643_count = 0;
static idx_t a2643_cap = 0;
static uint64_t a2643_k = 0;

seq_t a(idx_t n) {
    while (a2643_count < n) {
        uint64_t r, q;
        a2643_k++;
        r = a2643_k % 19;
        if (r != 7 && r != 11) continue;
        q = (a2643_k * a2643_k + a2643_k + 1) / 19;
        if (!a2643_isprime(q)) continue;
        if (a2643_count == a2643_cap) {
            idx_t nc = a2643_cap ? a2643_cap * 2 : 1024;
            uint64_t *nt = (uint64_t *)realloc(a2643_terms, (size_t)nc * sizeof(uint64_t));
            if (!nt) return (seq_t)0;
            a2643_terms = nt;
            a2643_cap = nc;
        }
        a2643_terms[a2643_count++] = a2643_k;
    }
    return (seq_t)a2643_terms[n - 1];
}
