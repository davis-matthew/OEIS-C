/*
 * A091181  A091180 indexed by A000040.
 * https://oeis.org/A091181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = pi(A091180(n)), where A091180 = primes q = 3*p - 2 with p
 *            prime; computed by a doubling sieve search
 * Range:     u8 n <= 44, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A091181_cache = NULL;
static size_t A091181_cnt = 0;
static uint64_t A091181_lim = 1024;

static void A091181_build(uint64_t L)
{
    unsigned char *comp = (unsigned char *)calloc((size_t)L + 1, 1);
    uint64_t i, j, pi = 0;
    size_t c = 0, cap = 0;
    seq_t *arr = NULL;
    if (!comp) return;
    comp[0] = comp[1] = 1;
    for (i = 2; i * i <= L; i++)
        if (!comp[i])
            for (j = i * i; j <= L; j += i)
                comp[j] = 1;
    for (i = 2; i <= L; i++) {
        if (comp[i]) continue;
        pi++;
        if ((i + 2) % 3 == 0 && !comp[(i + 2) / 3]) {
            if (c >= cap) {
                size_t nc = cap ? cap * 2 : 64;
                seq_t *np = mem_resize(seq_t, arr, nc);
                if (!np) { free(comp); return; }
                arr = np;
                cap = nc;
            }
            arr[c++] = (seq_t)pi;
        }
    }
    free(comp);
    if (A091181_cache) mem_free(A091181_cache);
    A091181_cache = arr;
    A091181_cnt = c;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    while ((idx_t)A091181_cnt < n) {
        A091181_build(A091181_lim);
        A091181_lim *= 2;
    }
    return A091181_cache[(size_t)(n - 1)];
}
