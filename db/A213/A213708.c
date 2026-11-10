/*
 * A213708  a(n) is the least inverse of A071542, i.e., minimal i such that
 *          A071542(i) = n.
 * https://oeis.org/A213708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least i with A071542(i) = n, where A071542(0)=0 and
 *            A071542(i) = 1 + A071542(i - popcount(i)); computed by an
 *            incremental sieve that records the first i at which each value
 *            appears
 * Range:     u8 n <= 68, u16 n <= 8727, u32 n <= 16500, u64 n <= 16500,
 *            u128 n <= 16500, big any n
 * Validated: standard, pass, 92 terms through n = 9754 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint32_t *A213708_steps = NULL;
static idx_t *A213708_first = NULL;
static idx_t A213708_nsteps = 0, A213708_cap = 0;
static idx_t A213708_nfirst = 0, A213708_fcap = 0;

seq_t a(idx_t n) {
    if (A213708_nsteps == 0) {
        A213708_cap = 1024;
        A213708_fcap = 1024;
        A213708_steps = (uint32_t *)malloc(sizeof(uint32_t) * A213708_cap);
        A213708_first = (idx_t *)malloc(sizeof(idx_t) * A213708_fcap);
        if (!A213708_steps || !A213708_first) abort();
        A213708_steps[0] = 0;
        A213708_first[0] = 0;
        A213708_nsteps = 1;
        A213708_nfirst = 1;
    }
    while (A213708_nfirst <= n) {
        idx_t i = A213708_nsteps;
        if (i >= A213708_cap) {
            A213708_cap <<= 1;
            A213708_steps = (uint32_t *)realloc(A213708_steps, sizeof(uint32_t) * A213708_cap);
            if (!A213708_steps) abort();
        }
        uint32_t v = A213708_steps[i - (idx_t)__builtin_popcountll((unsigned long long)i)] + 1;
        A213708_steps[i] = v;
        A213708_nsteps = i + 1;
        if (v == A213708_nfirst) {
            if (A213708_nfirst >= A213708_fcap) {
                A213708_fcap <<= 1;
                A213708_first = (idx_t *)realloc(A213708_first, sizeof(idx_t) * A213708_fcap);
                if (!A213708_first) abort();
            }
            A213708_first[A213708_nfirst++] = i;
        }
    }
    return (seq_t)A213708_first[n];
}
