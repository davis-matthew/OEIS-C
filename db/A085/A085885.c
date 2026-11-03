/*
 * A085885  Let r and s be such that r + s = n; a(n) = maximum value of phi(r)
 *          + phi(s).
 * https://oeis.org/A085885
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{1 <= r <= n-1} φ(r) + φ(n - r)
 * Range:     u8 n <= 217, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 89 terms through n = 971 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A search over all partitions; compute totients for 1..n-1 by sieve then take the maximum. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Allocate and free using malloc/free when compiled as C. */
static seq_t *alloc_cache(idx_t n)
{
    return mem_new(seq_t, n);
}

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;                /* offset starts at 2 */

    /* compute φ(0)..φ(n) using Euler's sieve */
    idx_t N = n + 1;
    seq_t *phi = alloc_cache(N);

    for (idx_t i = 0; i < N; ++i)        /* initialise φ(i) = i */
        phi[i] = (seq_t)i;
    phi[0] = 0;
    if (N > 1) phi[1] = 1;

    for (idx_t p = 2; p < N; ++p) {
        if (phi[p] == (seq_t)p) {        /* p is prime */
            for (idx_t j = p; j < N; j += p) {
                phi[j] -= phi[j] / (seq_t)p;
            }
        }
    }

    /* scan all partitions r + (n-r) to find the maximum sum */
    seq_t best = 0;
    for (idx_t r = 1; r <= n / 2; ++r) {
        seq_t val = phi[r] + phi[n - r];
        if (val > best) best = val;
    }

    free_seq_array(phi);
    return best;
}
