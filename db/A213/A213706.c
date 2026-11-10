/*
 * A213706  Partial sums of A071542.
 * https://oeis.org/A213706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(n) = a(n-1) + A071542(n), where A071542(0) = 0,
 *            A071542(n) = 1 + A071542(n - popcount(n))
 * Range:     u8 n <= 32, u16 n <= 718, u32 n <= 720, u64 n <= 720,
 *            u128 n <= 720, big any n
 * Validated: standard, pass, 77 terms through n = 686 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *A213706_steps = NULL;  /* A071542 values */
static seq_t *A213706_sums = NULL;   /* partial sums */
static idx_t  A213706_computed = 0;
static idx_t  A213706_allocated = 0;

static void A213706_ensure(idx_t n) {
    if (n + 1 > A213706_allocated) {
        idx_t new_alloc = A213706_allocated ? A213706_allocated : 16;
        while (new_alloc <= n) new_alloc <<= 1;
        seq_t *ns = mem_resize(seq_t, A213706_steps, new_alloc);
        if (!ns) abort();
        A213706_steps = ns;
        seq_t *nt = mem_resize(seq_t, A213706_sums, new_alloc);
        if (!nt) abort();
        A213706_sums = nt;
        A213706_allocated = new_alloc;
    }
    if (A213706_computed == 0) {
        A213706_steps[0] = 0;
        A213706_sums[0] = 0;
        A213706_computed = 1;
    }
    for (idx_t i = A213706_computed; i <= n; ++i) {
        idx_t sub = i - (idx_t)__builtin_popcountll((unsigned long long)i);
        A213706_steps[i] = A213706_steps[sub] + 1;
        A213706_sums[i] = A213706_sums[i - 1] + A213706_steps[i];
    }
    if (n + 1 > A213706_computed) A213706_computed = n + 1;
}

seq_t a(idx_t n) {
    A213706_ensure(n);
    return A213706_sums[n];
}
