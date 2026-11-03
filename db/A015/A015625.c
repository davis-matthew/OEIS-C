/*
 * A015625  Quadruples of different integers from [ 1,n ] with no common
 *          factors between triples.
 * https://oeis.org/A015625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of 4‑tuples of distinct integers from [1..n] such that
 *            no triple has a common divisor >1. Computed by
 *            inclusion–exclusion over primes
 * Range:     u8 n <= 10, u16 n <= 41, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-01)
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* Count 4‑tuples where every prime in the selected set appears in
   at least three of the four numbers. */
static void process_set(idx_t n, int t, const idx_t *ps, seq_t *total)
{
    int sign = (t % 2 == 0) ? 1 : -1;      /* inclusion–exclusion sign   */
    int M = 1 << t;                        /* number of masks for set S   */

    /* prod_mask[mask] : product of primes in mask           */
    idx_t prod_mask[32];
    prod_mask[0] = 1;
    for (int mask = 1; mask < M; ++mask) {
        idx_t prod = 1;
        for (int bit = 0; bit < t; ++bit)
            if (mask & (1 << bit))
                prod *= ps[bit];
        prod_mask[mask] = prod;
    }

    /* exact[mask] : numbers divisible by *exactly* the primes
       in mask (no other primes from the set).           */
    seq_t exact[32];
    for (int mask = 0; mask < M; ++mask)
        exact[mask] = n / prod_mask[mask];

    /* Möbius transform: subtract supersets of each mask       */
    for (int mask = M - 1; mask >= 0; --mask) {
        for (int sup = mask + 1; sup < M; ++sup)
            if ((sup & mask) == mask)
                exact[mask] -= exact[sup];
    }

    seq_t count_D = 0;
    int freq[32];

    /* Enumerate unordered 4‑tuples of masks (allow repetition) */
    for (int a = 0; a < M; ++a)
    for (int b = a; b < M; ++b)
    for (int c = b; c < M; ++c)
    for (int d = c; d < M; ++d) {
        for (int i = 0; i < M; ++i) freq[i] = 0;
        freq[a]++; freq[b]++; freq[c]++; freq[d]++;

        /* Each selected prime must be present in at least 3 numbers */
        int ok = 1;
        for (int p = 0; p < t; ++p) {
            int sum = 0;
            for (int mask = 0; mask < M; ++mask)
                if (mask & (1 << p))
                    sum += freq[mask];
            if (sum < 3) { ok = 0; break; }
        }
        if (!ok) continue;

        /* Verify enough numbers of each mask type are available */
        for (int i = 0; i < M; ++i)
            if (freq[i] > (int)exact[i]) { ok = 0; break; }
        if (!ok) continue;

        /* Count the number of ways to pick the specific numbers */
        seq_t term = 1;
        for (int i = 0; i < M; ++i) {
            int k = freq[i];
            if (k == 0) continue;
            if (k == 1) term *= exact[i];
            else if (k == 2) term *= exact[i] * (exact[i] - 1) / 2;
            else if (k == 3) term *= exact[i] * (exact[i] - 1) * (exact[i] - 2) / 6;
            else if (k == 4) term *= exact[i] * (exact[i] - 1) * (exact[i] - 2) *
                                      (exact[i] - 3) / 24;
        }
        count_D += term;
    }

    if (sign > 0) *total += count_D;
    else          *total -= count_D;
}

/* Recursive generation of all square‑free sets of primes <= n 
   where the product of the selected primes does not exceed n. */
static void dfs_primes(idx_t n, size_t start,
                       idx_t curr_prod,
                       const uint64_t *primes, size_t primes_count,
                       idx_t *sel, size_t sel_cnt,
                       seq_t *total)
{
    for (size_t i = start; i < primes_count; ++i) {
        if (curr_prod > n / primes[i]) break;
        idx_t prod = curr_prod * primes[i];
        sel[sel_cnt] = primes[i];
        process_set(n, (int)(sel_cnt + 1), sel, total);
        dfs_primes(n, i + 1, prod, primes,
                   primes_count, sel, sel_cnt + 1, total);
    }
}

seq_t a(idx_t n)
{
    /* total count of 4‑tuples with all numbers distinct */
    seq_t good = 0;
    if (n >= 4) {
        good = (seq_t)n * (n - 1) * (n - 2) * (n - 3) / 24;
    }

    uint64_t *prime_list = NULL;
    size_t prime_count = 0;
    build_primes_upto(n, &prime_list, &prime_count);

    /* t is never larger than 5 for n ≤ 10000 – array size 10 is safe */
    idx_t selected[10];

    dfs_primes(n, 0, 1, prime_list, prime_count,
               selected, 0, &good);

    free(prime_list);
    return good;
}
