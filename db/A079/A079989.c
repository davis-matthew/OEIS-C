/*
 * A079989  Number of permutations satisfying -k <= p(i) - i <= r and p(i) - i
 *          not in I, i=1..n, with k=3, r=3, I={1,2}.
 * https://oeis.org/A079989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 31, u64 n <= 60, u128 n <= 119,
 *            big any n
 * Helpers:   count_for_n, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A079989 (permutation‑counting sequence)
 *
 * Definition:  a(n) = number of permutations p of {1,…,n} such that
 *   –3 ≤ p(i) – i ≤ 3 and p(i) – i ∉ {1,2}  for all i = 1…n.
 *
 * Recurrence (valid for n ≥ 20):
 *   a(n) = a(n-1)+a(n-2)+2·a(n-3)+a(n-4)+3·a(n-5)+4·a(n-6)
 *          –7·a(n-7)–7·a(n-8)–6·a(n-9)+6·a(n-10)
 *          –2·a(n-11)–a(n-12)+a(n-13)+4·a(n-14)+a(n-15)
 *          –3·a(n-16)+a(n-18)–a(n-19)–a(n-20)
 *
 * Initial values (for n = 0…19):
 *   1, 1, 1, 1, 5, 13, 27, 51, 103, 221, 498,
 *   1064, 2240, 4728, 10076, 21559, 46075, 98085,
 *   208759, 444727
 *
 * Implementation detail:
 *   - For n ≤ 19 we recompute the value by a small
 *     dynamic‑programming DP over permutation masks.
 *   - For n ≥ 20 the recurrence is evaluated using a static
 *     cache that expands as needed.
 *   - All arithmetic is performed with the supplied `seq_t`,
 *     which may be an arbitrary‑precision type or an unsigned
 *     integer of various widths.
 *
 * The code obeys the interface required by the task:
 *   seq_t a(idx_t n)
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mul_small.h"
#include "../utils/count_for_n.h"

/* Coefficients for the recurrence: a(n) = Σ coeffs[i] · a(n-1-i)  (i=0…19) */
static const int coeffs[20] = {
    1,  1,  2,  1,  3,  4, -7, -7, -6,  6,
   -2, -1,  1,  4,  1, -3,  0,  1, -1, -1
};

/* Cache of computed terms.  cache[0] … cache[cache_size-1] are filled. */
static seq_t *cache      = NULL;
static idx_t cache_size  = 0;

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Ensure that cache contains terms up to index 'n'.  Grow cache if needed. */
static void ensure_cache(idx_t n)
{
    if (cache_size > n)
        return;

    idx_t new_size = n + 1;
    if (!cache)
    {
        cache = (seq_t*)malloc(new_size * sizeof(seq_t));
        if (!cache)
            abort();
    }
    else
    {
        cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!cache)
            abort();
    }

    /* Compute missing base terms (n ≤ 19) using the DP */
    for (idx_t i = cache_size; i <= n && i <= 19; ++i)
        cache[i] = count_for_n(i);

    /* Compute remaining terms via the recurrence */
    idx_t start = (cache_size > 19 ? cache_size : 20);
    for (idx_t i = start; i <= n; ++i)
    {
        seq_t pos = 0;
        seq_t neg = 0;
        for (int k = 0; k < 20; ++k)
        {
            int c = coeffs[k];
            if (c == 0)
                continue;
            idx_t idx = i - 1 - k;
            seq_t term = cache[idx];
            if (c > 0)
                pos += mul_small(term, (unsigned)(c));
            else
                neg += mul_small(term, (unsigned)(-c));
        }
        cache[i] = pos - neg;          /* guaranteed non‑negative */
    }

    cache_size = new_size;
}

/* ------------------------------------------------------------------ */
/* Public function required by the problem specification */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
