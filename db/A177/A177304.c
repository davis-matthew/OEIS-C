/*
 * A177304  Number of permutations of 6 copies of 1..n with all adjacent
 *          differences <= 1 in absolute value.
 * https://oeis.org/A177304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of permutations of six copies of 1..n with all
 *            adjacent differences <= 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t binom_small[7][7];          /* binomial coefficients up to 6 */
static int binom_initialized = 0;

static void init_binom(void)
{
    if (binom_initialized) return;
    binom_initialized = 1;
    for (int i = 0; i <= 6; ++i) {
        binom_small[i][0] = 1;
        binom_small[i][i] = 1;
        for (int j = 1; j < i; ++j)
            binom_small[i][j] = binom_small[i-1][j-1] + binom_small[i-1][j];
    }
}

seq_t a(idx_t n)
{
    /* a(0) = 1 (empty sequence) */
    if (n == 0) return 1;

    init_binom();

    /* dp[r][s1][s2] : number of valid sequences of 1..k
       where r = runs of current maximum (k), s1 = first element is k,
       s2 = last element is k.  r ranges 1..6.  */
    seq_t dp[7][2][2] = {{0}};
    seq_t nextdp[7][2][2] = {{0}};

    /* initial state for k = 1: a single run of six 1's, both ends 1. */
    dp[1][1][1] = 1;

    /* iterate k = 2 .. n */
    for (idx_t k = 2; k <= n; ++k) {
        /* clear nextdp */
        for (int r = 1; r <= 6; ++r)
            for (int s1 = 0; s1 <= 1; ++s1)
                for (int s2 = 0; s2 <= 1; ++s2)
                    nextdp[r][s1][s2] = 0;

        for (int r = 1; r <= 6; ++r) {
            int g_inside = 6 - r;   /* number of interior gaps of k-1 */
            for (int s1 = 0; s1 <= 1; ++s1) {
                for (int s2 = 0; s2 <= 1; ++s2) {
                    seq_t cnt = dp[r][s1][s2];
                    if (cnt == 0) continue;

                    for (int tin = 0; tin <= 1; ++tin) {
                        for (int tout = 0; tout <= 1; ++tout) {
                            /* only use boundary gaps when they are available */
                            if (tin > s1) continue;
                            if (tout > s2) continue;

                            for (int ui = 0; ui <= g_inside; ++ui) {
                                int u_total = ui + tin + tout;   /* total chosen gaps */
                                if (u_total == 0) continue;          /* at least one gap used */
                                if (u_total > 6) continue;            /* cannot distribute 6 copies */
                                if (u_total - 1 > 5) continue;        /* same check */

                                seq_t ways = cnt *
                                    binom_small[g_inside][ui] *
                                    binom_small[5][u_total-1];

                                int new_r  = u_total;   /* runs of new maximum k */
                                int new_s1 = tin;
                                int new_s2 = tout;

                                nextdp[new_r][new_s1][new_s2] += ways;
                            }
                        }
                    }
                }
            }
        }

        /* copy nextdp into dp for next iteration */
        for (int r = 1; r <= 6; ++r)
            for (int s1 = 0; s1 <= 1; ++s1)
                for (int s2 = 0; s2 <= 1; ++s2)
                    dp[r][s1][s2] = nextdp[r][s1][s2];
    }

    /* sum over all final states */
    seq_t result = 0;
    for (int r = 1; r <= 6; ++r)
        for (int s1 = 0; s1 <= 1; ++s1)
            for (int s2 = 0; s2 <= 1; ++s2)
                result += dp[r][s1][s2];

    return result;
}
