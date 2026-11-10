/*
 * A216496  Primes that are not in A094383.
 * https://oeis.org/A216496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th prime p such that no d>0 gives p-d, p-2d, p-3d all
 *            prime (search over a sieve)
 * Range:     u8 n <= 20, u16 n <= 25, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 25 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define A216496_LIMIT 20000

static uint32_t A216496_terms[256];
static size_t A216496_cnt = 0;
static bool A216496_done = false;

static void A216496_compute(void)
{
    char *comp = (char *)calloc(A216496_LIMIT + 1, 1);
    uint32_t i, j, p, d;
    comp[0] = comp[1] = 1;
    for (i = 2; i * i <= A216496_LIMIT; i++)
        if (!comp[i])
            for (j = i * i; j <= A216496_LIMIT; j += i) comp[j] = 1;
    for (p = 2; p <= A216496_LIMIT && A216496_cnt < 256; p++) {
        bool found = false;
        if (comp[p]) continue;
        for (d = 1; 3 * d + 2 <= p; d++) {
            if (!comp[p - d] && !comp[p - 2 * d] && !comp[p - 3 * d]) {
                found = true;
                break;
            }
        }
        if (!found) A216496_terms[A216496_cnt++] = p;
    }
    free(comp);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (!A216496_done) {
        A216496_compute();
        A216496_done = true;
    }
    if (n > (idx_t)A216496_cnt) return (seq_t)0;
    return (seq_t)A216496_terms[n - 1];
}
