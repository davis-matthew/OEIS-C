/*
 * A215876  Numbers that contain each decimal digit at least twice.
 * https://oeis.org/A215876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = 10^19 + k‑th lexicographic permutation
   of the multiset {1:1, 2:2, …, 9:2}, where k = n‑1.
   The first 17!/2^8 ≈ 1.4×10^12 terms of A215876 are generated
   by this routine. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/pow10.h"

/* factorials up to 17! for combinatorial counts */
static unsigned long long fact[18] = {0};
static bool fact_initialized = false;

static void init_fact(void)
{
    if (fact_initialized) return;
    fact[0] = 1ULL;
    for (int i = 1; i <= 17; ++i)
        fact[i] = fact[i-1] * (unsigned long long)i;
    fact_initialized = true;
}

seq_t a(idx_t n)
{
    /* k = 0‑based rank of the desired permutation */
    unsigned long long k = (unsigned long long)(n - 1ULL);
    init_fact();

    /* multiset: 1 appears once, digits 2–9 twice, digit 0 never used */
    int counts[10] = {0};
    counts[1] = 1;
    for (int d = 2; d <= 9; ++d) counts[d] = 2;

    const int LEN = 17;          /* multiset size */
    int perm[LEN];
    int remaining = LEN;

    /* Unrank the permutation */
    for (int pos = 0; pos < LEN; ++pos) {
        for (int d = 0; d <= 9; ++d) {
            if (counts[d] == 0) continue;

            counts[d]--;                     /* tentatively take d */
            unsigned long long perms = fact[remaining - 1];
            for (int p = 0; p <= 9; ++p)
                perms /= fact[counts[p]];     /* count permutations of remaining */

            if (k >= perms) {
                k -= perms;                   /* skip this block */
                counts[d]++;                  /* restore */
            } else {
                perm[pos] = d;                /* correct digit chosen */
                --remaining;
                break;
            }
        }
    }

    /* Assemble the final number: 10^19 + suffix from perm */
    seq_t result = pow10(19);   /* 10^19 */
    seq_t suffix = 0;
    for (int i = 0; i < LEN; ++i)
        suffix = suffix * 10 + (seq_t)perm[i];

    result += suffix;
    return result;
}
