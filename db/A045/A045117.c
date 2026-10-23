/*
 * A045117  Numbers whose base-4 representation contains exactly one 1 and
 *          three 3's.
 * https://oeis.org/A045117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base‑4 representation contains exactly one 1 and
 *            three 3's. Implementation: generate all such numbers up to
 *            base‑4 length 10, sort them, and return the n-th (1‑based) value
 * Range:     u8 none, u16 none, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Comparator for seq_t values.  Works for integral or arbitrary‑precision seq_t. */
static int seq_t_cmp(const void *a, const void *b)
{
    const seq_t *sa = (const seq_t *)a;
    const seq_t *sb = (const seq_t *)b;
    if (*sa < *sb) return -1;
    if (*sa > *sb) return 1;
    return 0;
}

#define CAP 80000                 /* sufficient for all n needed (n ≤ 10000) */
static seq_t seq_cache[CAP];
static size_t seq_cache_len = 0;
static bool seq_cache_built = false;

/* Generate the complete sequence up to base‑4 length 10. */
static void build_sequence(void)
{
    if (seq_cache_built) return;

    const int Lmax = 10;          /* basis length sufficient for all required n */

    for (int L = 4; L <= Lmax; ++L)
    {
        /* pre‑compute powers of 4 for this length */
        seq_t powers[L];
        powers[0] = (seq_t)1;
        for (int i = 1; i < L; ++i)
            powers[i] = powers[i - 1] * (seq_t)4;

        uint32_t all_masks = 1u << L;

        for (uint32_t mask1 = 0; mask1 < all_masks; ++mask1)
        {
            if (__builtin_popcount(mask1) != 4) continue;

            /* collect the 4 positions that carry 1 or 3 */
            int pos[4];
            int ppos = 0;
            for (int i = 0; i < L; ++i)
                if (mask1 & (1u << i))
                    pos[ppos++] = i;

            /* collect the remaining positions */
            int restpos[16];
            int restcnt = 0;
            uint32_t restmask = ((1u << L) - 1u) ^ mask1;
            for (int i = 0; i < L; ++i)
                if (restmask & (1u << i))
                    restpos[restcnt++] = i;

            /* choose which of the 4 positions holds the single digit 1 */
            for (int oneidx = 0; oneidx < 4; ++oneidx)
            {
                seq_t base_num = (seq_t)0;
                base_num += (seq_t)1 * powers[pos[oneidx]];
                for (int j = 0; j < 4; ++j)
                    if (j != oneidx)
                        base_num += (seq_t)3 * powers[pos[j]];

                uint32_t assign_total = 1u << restcnt;
                for (uint32_t assignMask = 0; assignMask < assign_total; ++assignMask)
                {
                    seq_t val = base_num;
                    bool bad = false;
                    for (int t = 0; t < restcnt; ++t)
                    {
                        if (assignMask & (1u << t))
                            val += (seq_t)2 * powers[restpos[t]];
                        else if (restpos[t] == L - 1)   /* leading zero not allowed */
                        {
                            bad = true;
                            break;
                        }
                    }
                    if (bad) continue;
                    seq_cache[seq_cache_len++] = val;
                }
            }
        }
    }

    /* sort the generated terms */
    qsort(seq_cache, seq_cache_len, sizeof(seq_t), seq_t_cmp);
    seq_cache_built = true;
}

/* External function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined offset; return 0 */

    build_sequence();                      /* generate and sort once */

    /* the problem guarantees n will be within the generated range */
    return seq_cache[(size_t)n - 1];
}
