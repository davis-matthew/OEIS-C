/*
 * A061133  Number of degree-n even permutations of order exactly 6.
 * https://oeis.org/A061133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of degree‑n even permutations of order exactly 6
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   falling_fact
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* realloc, calloc, size_t */
#include "../utils/falling_fact.h"

static seq_t *dp_tot_1236 = NULL;   /* all permutations with cycles 1,2,3,6   */
static seq_t *dp_even_1236 = NULL;  /* even such permutations                */
static seq_t *dp_odd_1236 = NULL;   /* odd   such permutations                */
static seq_t *dp_tot_12   = NULL;   /* all permutations with cycles 1,2       */
static seq_t *dp_even_12  = NULL;   /* even such permutations                */
static seq_t *dp_odd_12   = NULL;   /* odd   such permutations                */
static seq_t *dp_tot_13   = NULL;   /* all permutations with cycles 1,3       */
static idx_t  dp_max_n    = 0;       /* largest n already processed            */

/* Extend all tables up to target n.  All tables share the same size. */
static void extend_tables(idx_t n)
{
    if (n <= dp_max_n)
        return;

    size_t new_len = (size_t)(n + 1);

    dp_tot_1236 = (seq_t *)realloc(dp_tot_1236, new_len * sizeof(seq_t));
    dp_even_1236 = (seq_t *)realloc(dp_even_1236, new_len * sizeof(seq_t));
    dp_odd_1236 = (seq_t *)realloc(dp_odd_1236, new_len * sizeof(seq_t));

    dp_tot_12 = (seq_t *)realloc(dp_tot_12, new_len * sizeof(seq_t));
    dp_even_12 = (seq_t *)realloc(dp_even_12, new_len * sizeof(seq_t));
    dp_odd_12 = (seq_t *)realloc(dp_odd_12, new_len * sizeof(seq_t));

    dp_tot_13 = (seq_t *)realloc(dp_tot_13, new_len * sizeof(seq_t));

    /* Base cases (for n = 0) */
    dp_tot_1236[0] = (seq_t)1;
    dp_even_1236[0] = (seq_t)1;
    dp_odd_1236[0] = (seq_t)0;

    dp_tot_12[0] = (seq_t)1;
    dp_even_12[0] = (seq_t)1;
    dp_odd_12[0] = (seq_t)0;

    dp_tot_13[0] = (seq_t)1;

    const int len_set1[4] = {1, 2, 3, 6};
    const int len_set2[2] = {1, 2};
    const int len_set3[2] = {1, 3};

    for (idx_t m = dp_max_n + 1; m <= n; ++m) {
        /* ----- Set {1,2,3,6} ----- */
        seq_t tot1236 = 0;
        for (int i = 0; i < 4; ++i) {
            int l = len_set1[i];
            if ((idx_t)l > m) continue;
            seq_t f = falling_fact(m - 1, (idx_t)(l - 1));
            tot1236 += f * dp_tot_1236[m - l];
        }
        dp_tot_1236[m] = tot1236;

        seq_t even1236 = 0, odd1236 = 0;
        for (int i = 0; i < 4; ++i) {
            int l = len_set1[i];
            if ((idx_t)l > m) continue;
            seq_t f = falling_fact(m - 1, (idx_t)(l - 1));
            if ((l & 1) == 0) {            /* even cycle length */
                even1236 += f * dp_odd_1236[m - l];
                odd1236  += f * dp_even_1236[m - l];
            } else {                       /* odd cycle length */
                even1236 += f * dp_even_1236[m - l];
                odd1236  += f * dp_odd_1236[m - l];
            }
        }
        dp_even_1236[m] = even1236;
        dp_odd_1236[m]  = odd1236;

        /* ----- Set {1,2} ----- */
        seq_t tot12 = 0;
        for (int i = 0; i < 2; ++i) {
            int l = len_set2[i];
            if ((idx_t)l > m) continue;
            seq_t f = falling_fact(m - 1, (idx_t)(l - 1));
            tot12 += f * dp_tot_12[m - l];
        }
        dp_tot_12[m] = tot12;

        seq_t even12 = 0, odd12 = 0;
        for (int i = 0; i < 2; ++i) {
            int l = len_set2[i];
            if ((idx_t)l > m) continue;
            seq_t f = falling_fact(m - 1, (idx_t)(l - 1));
            if ((l & 1) == 0) {            /* even cycle length (2) */
                even12 += f * dp_odd_12[m - l];
                odd12  += f * dp_even_12[m - l];
            } else {                       /* odd cycle length (1) */
                even12 += f * dp_even_12[m - l];
                odd12  += f * dp_odd_12[m - l];
            }
        }
        dp_even_12[m] = even12;
        dp_odd_12[m]  = odd12;

        /* ----- Set {1,3} (only odd cycles) ----- */
        seq_t tot13 = 0;
        for (int i = 0; i < 2; ++i) {
            int l = len_set3[i];
            if ((idx_t)l > m) continue;
            seq_t f = falling_fact(m - 1, (idx_t)(l - 1));
            tot13 += f * dp_tot_13[m - l];
        }
        dp_tot_13[m] = tot13;
    }

    dp_max_n = n;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* For n < 7 the sequence is 0 by definition. */
    if (n < 7) return (seq_t)0;

    extend_tables(n);

    seq_t even1236 = dp_even_1236[n];
    seq_t tot13    = dp_tot_13[n];
    seq_t even12   = dp_even_12[n];

    /* Subtract permutations with lcm 1, 2, or 3.  The identity is counted
       in both subtractions, so we add 1 to correct the double subtraction. */
    seq_t res = even1236;
    if (res >= tot13) res -= tot13; else res = 0;
    if (res >= even12) res -= even12; else res = 0;
    res += 1;                      /* correct the double subtraction of the identity */

    return res;
}
