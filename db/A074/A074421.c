/*
 * A074421  Number of 5-ary Lyndon words of length n over Z_5 with trace 1 and
 *          subtrace 4.
 * https://oeis.org/A074421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) = number of 5‑ary Lyndon words of length n over Z_5 with trace 1 
 *        and subtrace 4.  The code counts primitive words by dynamic 
 *        programming over all traces and subtraces, then divides the
 *        primitive count by n (each class of n cyclically‑equivalent
 *        primitive words contributes exactly one Lyndon word). 
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* idx_t and seq_t are defined by the build environment.  Assume they
   support the arithmetic operators listed in the requirements.  */
#ifndef IDX_T
typedef uint64_t idx_t;
#endif
#ifndef SEQ_T
/* seq_t is defined elsewhere. */
#endif

/* maximum n handled by this implementation (adjustable). */
#define MAX_N 10000

/* Two‑dimensional tables for the number of words with given trace and
   subtrace.  The tables are indexed by word length and then by
   (trace, subtrace).  The values fit into seq_t. */
static seq_t C[MAX_N + 1][5][5];
static seq_t P[MAX_N + 1][5][5];

seq_t a(idx_t n)
{
    /* a(1) is the first term in the OEIS list, a(0) is undefined. */
    if (n == 0 || n > MAX_N)
        return 0;

    /* Temporary tables for the recurrence that builds C[|w|] from C[|w|-1]. */
    seq_t cur[5][5] = {{0}};
    seq_t nxt[5][5] = {{0}};

    /* Word of length 0: trace = 0, subtrace = 0. */
    cur[0][0] = 1;
    C[0][0][0] = 1;

    for (idx_t len = 1; len <= n; ++len) {
        /* reset next table */
        for (int t = 0; t < 5; ++t)
            for (int s = 0; s < 5; ++s)
                nxt[t][s] = 0;

        /* Build C[len] from C[len-1] */
        for (int t = 0; t < 5; ++t) {
            for (int s = 0; s < 5; ++s) {
                seq_t val = cur[t][s];
                if (!val) continue;
                for (int d = 0; d < 5; ++d) {
                    int t2 = (t + d) % 5;
                    int s2 = (s + (d * t) % 5) % 5;
                    nxt[t2][s2] += val;
                }
            }
        }

        /* store the counts for this length */
        for (int t = 0; t < 5; ++t)
            for (int s = 0; s < 5; ++s)
                C[len][t][s] = nxt[t][s];

        /* Möbius inversion to obtain primitive counts P[len] */
        seq_t cons[5][5] = {{0}};

        /* Iterate over all proper divisors d of len */
        for (idx_t d = 1; d * d <= len; ++d) {
            if (len % d != 0) continue;
            /* first divisor d */
            if (d < len) {
                idx_t r = len / d;
                int r_mod5 = r % 5;
                int r_choose_2 = ((int)((r * (r - 1) / 2) % 5));
                for (int a = 0; a < 5; ++a)
                    for (int b = 0; b < 5; ++b) {
                        seq_t pv = P[d][a][b];
                        if (!pv) continue;
                        int t = (r_mod5 * a) % 5;
                        int s = (r_mod5 * b + r_choose_2 * ((a * a) % 5)) % 5;
                        cons[t][s] += pv;
                    }
            }

            /* second divisor len/d if distinct */
            idx_t d2 = len / d;
            if (d2 != d && d2 < len) {
                idx_t r = len / d2;              /* r = d */
                int r_mod5 = r % 5;
                int r_choose_2 = ((int)((r * (r - 1) / 2) % 5));
                for (int a = 0; a < 5; ++a)
                    for (int b = 0; b < 5; ++b) {
                        seq_t pv = P[d2][a][b];
                        if (!pv) continue;
                        int t = (r_mod5 * a) % 5;
                        int s = (r_mod5 * b + r_choose_2 * ((a * a) % 5)) % 5;
                        cons[t][s] += pv;
                    }
            }
        }

        /* subtract contributions from shorter periods */
        for (int t = 0; t < 5; ++t)
            for (int s = 0; s < 5; ++s)
                P[len][t][s] = C[len][t][s] - cons[t][s];

        /* prepare for next iteration */
        for (int i = 0; i < 5; ++i)
            for (int j = 0; j < 5; ++j)
                cur[i][j] = nxt[i][j];
    }

    /* Each primitive class of length n contributes exactly one Lyndon word.
       Therefore divide the primitive count by n. */
    return P[n][1][4] / (seq_t)n;
}
