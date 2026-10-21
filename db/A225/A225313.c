/*
 * A225313  Number of 6Xn -1,1 arrays such that the sum over i=1..6,j=1..n of
 *          i*x(i,j) is zero and rows are nondecreasing (ways to put n
 *          thrusters pointing east or west at each of 6 positions 1..n
 *          distance from the hinge of a south-pointing gate without turning
 *          the gate).
 * https://oeis.org/A225313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            a(n-2)+a(n-4)-a(n-10)-2*a(n-14)+a(n-18)+a(n-20)+a(n-22)+a(n-24)
 *            -2*a(n-28)-a(n-32)+a(n-38)+a(n-40)-a(n-42) Initial values:
 *            a(1)=0, a(2)=35, a(3)=0, a(4)=443, a(5)=0, a(6)=2341, a(7)=0,
 *            a(8)=8221, a(9)=0, a(10)=22351, … (as listed in OEIS A225313)
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 125, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   compute_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>   /* for abort() */
#include "../utils/compute_small.h"

/* --------------------------------------------------------------------- */
/*  Compute a(n) for n ≤ 42 by dynamic programming over the 6×n−1,1 matrix */

/* --------------------------------------------------------------------- */
/*  Cached evaluation of the sequence                                         */
/* --------------------------------------------------------------------- */

#define MAX_CACHE 10001          /* we pre‑allocate up to n = 10000 */
static seq_t cache[MAX_CACHE];
static bool  computed[MAX_CACHE] = {0};

/**
 * Return a(n) for any n ≥ 1.
 * The routine works for all supported idx_t and seq_t types,
 * including the arbitrary‑precision build.
 */
seq_t a(idx_t n)
{
    /* --------------------------------------------------------------- */
    /*  Fast path: use the pre‑allocated static cache                   */
    /* --------------------------------------------------------------- */
    if (n < MAX_CACHE) {
        for (size_t i = 1; i <= (size_t)n; ++i) {
            if (computed[i]) continue;

            if (i <= 42) {
                cache[i] = compute_small((idx_t)i);
            } else {
                seq_t pos = cache[i-2] + cache[i-4] + cache[i-18] + cache[i-20] +
                            cache[i-22] + cache[i-24] + cache[i-38] + cache[i-40];
                seq_t neg = cache[i-10] + 2 * cache[i-14] + 2 * cache[i-28] +
                            cache[i-32] + cache[i-42];
                cache[i] = pos - neg;          /* pos ≥ neg for all known terms */
            }
            computed[i] = true;
        }
        return cache[(size_t)n];
    }

    /* --------------------------------------------------------------- */
    /*  For n ≥ MAX_CACHE fall back to a dynamic cache                */
    /* --------------------------------------------------------------- */
    static seq_t *dyn_cache   = NULL;
    static bool  *dyn_computed = NULL;
    static size_t dyn_cap      = 0;

    size_t N = (size_t)n + 1;
    if (N > dyn_cap) {
        size_t new_cap = dyn_cap ? dyn_cap * 2 : 64;
        if (new_cap < N) new_cap = N;
        seq_t *c   = (seq_t  *)realloc(dyn_cache,   new_cap * sizeof(seq_t));
        bool  *pc  = (bool  *)realloc(dyn_computed,new_cap * sizeof(bool));
        if (!c || !pc) abort();          /* unrecoverable error */
        for (size_t i = dyn_cap; i < new_cap; ++i) {
            c[i] = 0;
            pc[i] = false;
        }
        dyn_cache   = c;
        dyn_computed = pc;
        dyn_cap     = new_cap;
    }

    for (size_t i = 1; i <= n; ++i) {
        if (dyn_computed[i]) continue;

        if (i <= 42) {
            dyn_cache[i] = compute_small((idx_t)i);
        } else {
            seq_t pos = dyn_cache[i-2] + dyn_cache[i-4] + dyn_cache[i-18] + dyn_cache[i-20] +
                        dyn_cache[i-22] + dyn_cache[i-24] + dyn_cache[i-38] + dyn_cache[i-40];
            seq_t neg = dyn_cache[i-10] + 2 * dyn_cache[i-14] + 2 * dyn_cache[i-28] +
                        dyn_cache[i-32] + dyn_cache[i-42];
            dyn_cache[i] = pos - neg;
        }
        dyn_computed[i] = true;
    }
    return dyn_cache[(size_t)n];
}
