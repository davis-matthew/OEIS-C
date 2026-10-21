/*
 * A093469  Number of 6 X 6 binary matrices with n ones, distinct up to cyclic
 *          shifts of rows and/or columns; reflection through any vertical or
 *          horizontal axis; and reflection through the main diagonal. Also
 *          number of quasi-n-ominoes on a torus divided into a 6 X 6 grid.
 * https://oeis.org/A093469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 14, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   apply_perm
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A003***   OEIS A093469
    Count of 6×6 binary matrices with n ones, up to
    cyclic shifts of rows/columns, reflections over
    vertical/horizontal axes, and over the main diagonal.
    Computed by Burnside's lemma: |orbit| = 1/|G| ∑_g count_fixed(g,n)
    with |G| = 288.  */

/*  dependencies: seq_t, idx_t are supplied by the build environment.  */
#include <stdbool.h>
#include "../utils/apply_perm.h"

static seq_t cache[37];
static bool ready = false;

static void compute(void)
{
    const int N = 36;                /* number of cells */
    seq_t tot[37] = {0};             /* accumulate over group elements   */
    int group_cnt = 0;

    for (int kr = 0; kr < 6; ++kr)
    for (int kc = 0; kc < 6; ++kc)
    for (int vb = 0; vb < 2; ++vb)
    for (int hb = 0; hb < 2; ++hb)
    for (int db = 0; db < 2; ++db)
    {
        /* orbit size multiset for this group element */
        int orbit_sz[37] = {0};
        bool seen[36] = {0};

        for (int i = 0; i < N; ++i)
        {
            if (!seen[i])
            {
                int cur = i;
                int len = 0;
                do
                {
                    seen[cur] = true;
                    cur = apply_perm(cur, kr, kc, vb, hb, db);
                    ++len;
                } while (cur != i);
                ++orbit_sz[len];
            }
        }

        /* DP to count ways to reach each possible number of ones */
        seq_t ways[37] = {0};
        ways[0] = 1;           /* empty set of orbits */

        for (int s = 1; s <= N; ++s)
        {
            int cnt = orbit_sz[s];
            if (cnt == 0) continue;
            /* multiply the polynomial (1 + x^s)^cnt */
            for (int k = 0; k < cnt; ++k)
            {
                for (int w = N; w >= s; --w)
                {
                    ways[w] += ways[w - s];
                }
            }
        }

        /* accumulate for Burnside */
        for (int w = 0; w <= N; ++w) tot[w] += ways[w];
        ++group_cnt;
    }

    /* divide by |G| = 288 */
    for (int w = 0; w <= N; ++w) cache[w] = tot[w] / 288;

    ready = true;
}

seq_t a(idx_t n)
{
    if (n > 36) return 0;
    if (!ready) compute();
    return cache[n];
}
