/*
 * A002314  Minimal integer square root of -1 modulo p, where p is the n-th
 *          prime of the form 4k+1.
 * https://oeis.org/A002314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min(x, p-x) where p is the n-th prime == 1 (mod 4) (found
 *            by search) and x = c^((p-1)/4) mod p for any quadratic
 *            non-residue c of p (so x^2 == -1 mod p)
 * Range:     u8 n <= 61, u16 n <= 6194, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a2314_isprime, a2314_powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/a2314_isprime.h"
#include "../utils/a2314_powmod.h"

static uint64_t *a2314_pr = NULL;
static idx_t a2314_cnt = 0, a2314_cap = 0;

static uint64_t a2314_nth(idx_t n)
{
    uint64_t p;
    if (a2314_cnt >= n) return a2314_pr[n - 1];
    p = a2314_cnt ? a2314_pr[a2314_cnt - 1] + 4 : 5;
    while (a2314_cnt < n) {
        if (a2314_isprime(p)) {
            if (a2314_cnt == a2314_cap) {
                idx_t nc = a2314_cap ? a2314_cap * 2 : 1024;
                uint64_t *np = (uint64_t *)realloc(a2314_pr, (size_t)nc * sizeof(uint64_t));
                if (!np) return 0;
                a2314_pr = np;
                a2314_cap = nc;
            }
            a2314_pr[a2314_cnt++] = p;
        }
        p += 4;
    }
    return a2314_pr[n - 1];
}

seq_t a(idx_t n)
{
    uint64_t p, c, x;
    if (n < 1) return 0;
    p = a2314_nth(n);
    for (c = 2; c < p; c++)
        if (a2314_powmod(c, (p - 1) / 2, p) == p - 1) break;
    x = a2314_powmod(c, (p - 1) / 4, p);
    if (p - x < x) x = p - x;
    return (seq_t)x;
}
