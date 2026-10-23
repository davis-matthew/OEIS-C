/*
 * A048926  Numbers that are the sum of 5 positive cubes in exactly 1 way.
 * https://oeis.org/A048926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 9856, u64 n <= 9856, u128 n <= 9856,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A048926
   Numbers that are the sum of 5 positive cubes in exactly 1 way.
   The function builds an unordered “coin‑change” table using the cube
   values 1³,2³,…,120³ (since 120³ > 1685758) and collects all sums
   that can be written in exactly one unordered quintuple of cubes.
   The resulting list is stored once in a static array and reused
   for any future calls. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_VAL     1685758   /* last term of the sequence */
#define MAX_ROOT    120        /* 120³ = 1728000 > MAX_VAL */
#define MAX_TERMS   11062     /* number of terms in the sequence */

seq_t a(idx_t n)
{
    /* static cache */
    static bool initialized = false;
    static seq_t *seq = NULL;
    static size_t seq_len = 0;

    if (!initialized) {
        /* 5 1‑dimensional tables for 0..5 cubes */
        unsigned int *dp[6];
        for (int k = 0; k <= 5; ++k) {
            dp[k] = (unsigned int*)calloc((size_t)MAX_VAL + 1, sizeof(unsigned int));
            if (!dp[k]) exit(1);
        }
        dp[0][0] = 1;  /* one way to get sum 0 with 0 cubes */

        /* pre‑compute cube values */
        seq_t cube[MAX_ROOT + 1];
        for (int i = 1; i <= MAX_ROOT; ++i)
            cube[i] = (seq_t)i * (seq_t)i * (seq_t)i;

        /* coin‑change DP: unordered combinations of cubes */
        for (int v = 1; v <= MAX_ROOT; ++v) {
            seq_t cv = cube[v];
            int cv_int = (int)cv;  /* cv ≤ MAX_VAL ≤ 2³¹‑1 */
            for (int k = 1; k <= 5; ++k) {
                unsigned int *prev = dp[k-1];
                unsigned int *cur  = dp[k];
                for (int s = cv_int; s <= MAX_VAL; ++s)
                    cur[s] += prev[s - cv_int];
            }
        }

        /* allocate static array for results */
#ifdef __cplusplus
        seq = new seq_t[MAX_TERMS];
#else
        seq = (seq_t*)calloc(MAX_TERMS, sizeof(seq_t));
#endif
        if (!seq) exit(1);

        /* collect all numbers with exactly one representation */
        for (int s = 1; s <= MAX_VAL; ++s)
            if (dp[5][s] == 1)
                seq[seq_len++] = (seq_t)s;

        /* clean up DP tables */
        for (int k = 0; k <= 5; ++k) free(dp[k]);

        initialized = true;
    }

    /* return the requested term if in bounds */
    return (n >= 1 && (size_t)n <= seq_len) ? seq[n-1] : (seq_t)0;
}
