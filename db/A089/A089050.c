/*
 * A089050  Number of ways of writing n as a sum of exactly 5 powers of 2.
 * https://oeis.org/A089050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered multisets {2^a,2^b,2^c,2^d,2^e} of
 *            five powers of 2 (a,b,c,d,e ≥ 0) whose sum equals n
 * Range:     u8 n <= 9569, u16 n <= 9569, u32 n <= 9569, u64 n <= 9569,
 *            u128 n <= 9569, big any n
 * Helpers:   compute_generic
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/compute_generic.h"

#define GLOBAL_MAX 10000

static seq_t cache[GLOBAL_MAX + 1];
static bool initialized = false;

static void precompute(void) {
    /* pre‑compute all values up to GLOBAL_MAX by enumerating
       all non‑decreasing 5‑tuples of exponents. */
    int maxExp = 0;
    while (((unsigned long long)1 << (maxExp + 1)) <= (unsigned long long)GLOBAL_MAX) {
        ++maxExp;
    }
    unsigned long long pow2[64];
    pow2[0] = 1ULL;
    for (int e = 1; e <= maxExp; ++e) {
        pow2[e] = pow2[e - 1] << 1;
    }

    for (int e1 = 0; e1 <= maxExp; ++e1) {
        unsigned long long p1 = pow2[e1];
        for (int e2 = e1; e2 <= maxExp; ++e2) {
            unsigned long long p2 = pow2[e2];
            for (int e3 = e2; e3 <= maxExp; ++e3) {
                unsigned long long p3 = pow2[e3];
                for (int e4 = e3; e4 <= maxExp; ++e4) {
                    unsigned long long p4 = pow2[e4];
                    for (int e5 = e4; e5 <= maxExp; ++e5) {
                        unsigned long long sum =
                            p1 + p2 + p3 + p4 + pow2[e5];
                        if (sum <= GLOBAL_MAX) {
                            cache[(idx_t)sum] = cache[(idx_t)sum] + 1;
                        }
                    }
                }
            }
        }
    }
}

seq_t a(idx_t n) {
    if (!initialized) {
        /* Initialise cache of values up to GLOBAL_MAX */
        for (int i = 0; i <= GLOBAL_MAX; ++i) cache[i] = 0;
        precompute();
        initialized = true;
    }
    if (n <= GLOBAL_MAX) {
        return cache[(idx_t)n];
    }
    /* Falls back to on‑the‑fly computation for larger n. */
    return compute_generic(n);
}
