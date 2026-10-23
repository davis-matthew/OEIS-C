/*
 * A116489  Number of factorizations of n into odious numbers > 1.
 * https://oeis.org/A116489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of factorizations of n into odious (>0) divisors,
 *            counting the single trivial factorization 1*n when n itself is
 *            odious
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* check if a number has an odd number of 1‑bits (odious) */
static inline bool is_odious_uint64(uint64_t x)
{
    return (__builtin_popcountll(x) & 1) != 0;
}

/* generate all divisors of a number from its prime factorisation */
static void gen_divs_rec(size_t idx, uint64_t cur,
                         const uint64_t *pr, const uint64_t *ex,
                         size_t cnt, size_t *outcnt, uint64_t *out)
{
    if (idx == cnt) {
        out[*outcnt] = cur;
        ++(*outcnt);
        return;
    }
    uint64_t v = cur;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        gen_divs_rec(idx + 1, v, pr, ex, cnt, outcnt, out);
        v *= pr[idx];
    }
}

/* comparator for qsort */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t av = *(const uint64_t *)a;
    uint64_t bv = *(const uint64_t *)b;
    return (av < bv) ? -1 : (av > bv);
}

/* recursive DFS counting factorizations into odious >1 factors.
   depth = number of factors already chosen. */
static seq_t dfs(uint64_t rem, uint64_t min_factor, uint8_t depth)
{
    seq_t res = 0;

    /* factorise rem */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64(rem, pr, ex, &cnt);

    /* generate all divisors */
    uint64_t divs[256];
    size_t divcnt = 0;
    gen_divs_rec(0, 1, pr, ex, cnt, &divcnt, divs);

    /* sort divisors to enforce non‑decreasing order */
    qsort(divs, divcnt, sizeof(uint64_t), cmp_uint64);

    /* iterate over divisors */
    for (size_t i = 0; i < divcnt; ++i) {
        uint64_t d = divs[i];
        if (d < min_factor) continue;
        if (d < 2) continue;                /* skip 1, handled separately */
        if (!is_odious_uint64(d)) continue;  /* only odious factors >1 */

        if (d == rem) {
            if (depth + 1 >= 2)             /* at least two factors */
                res += (seq_t)1;
        } else {
            res += dfs(rem / d, d, depth + 1);
        }
    }
    return res;
}

/* external function required by the problem */
seq_t a(idx_t n)
{
    if (n == 1)           /* special case: only 1 factorization 1 */
        return (seq_t)1;

    bool n_odious = is_odious_uint64((uint64_t)n);

    seq_t count = dfs((uint64_t)n, 2, 0);   /* factorisations of n into >1 odious factors */

    if (n_odious)          /* add the trivial 1*n factorisation */
        count += (seq_t)1;

    return count;
}
