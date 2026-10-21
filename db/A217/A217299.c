/*
 * A217299  Number of height minimal AVL trees with n (leaf-) nodes.
 * https://oeis.org/A217299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of height‑minimal AVL trees with n leaf nodes.
 *            Computed by dynamic programming using the Fibonacci recurrence
 *            for the minimal number of leaves of a tree of given height. For
 *            each height h we store how many AVL trees have exactly n leaves
 *            and exactly height h
 * Range:     u8 n <= 16, u16 n <= 32, u32 n <= 64, u64 n <= 128,
 *            u128 n <= 167, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*  static state  */
static seq_t **dp   = NULL;   /* dp[h][n] – number of AVL trees of height h and n leaves   */
static idx_t  *minL = NULL;   /* minL[h] – minimal number of leaves for height h         */
static size_t  dp_height = 0; /* maximum height computed                               */
static size_t  dp_nmax   = 0; /* maximum leaf count computed for that height set        */
static size_t  minL_size = 0; /* allocated size of minL                                */

/*  free previously allocated tables  */
static void free_dp(void)
{
    if (dp != NULL) {
        for (size_t h = 0; h < dp_height; ++h) {
            free(dp[h]);
        }
        free(dp);
        dp = NULL;
    }
    if (minL != NULL) {
        free(minL);
        minL = NULL;
    }
    dp_height = 0;
    dp_nmax   = 0;
    minL_size = 0;
}

/*  recompute DP tables up to nmax  (called only when nmax > dp_nmax)  */
static void recompute_dp(idx_t nmax)
{
    if (nmax == 0) return;

    free_dp();                       /* clear old data */

    /*  compute Fibonacci numbers up to > nmax  */
    seq_t a = (seq_t)1;          /* F1 */
    seq_t b = (seq_t)1;          /* F2 */
    seq_t next;

    /*  minL[0] = F2 = 1, minL[1] = F3 = 2  */
    minL_size = 2;
    minL = (idx_t *)malloc(minL_size * sizeof(idx_t));
    minL[0] = (idx_t)1;   /* F2 */
    minL[1] = (idx_t)2;   /* F3 */

    size_t h = 0;          /* height index 0 already filled */

    while (true) {
        next = a + b;      /* F_{k+2} */
        a = b;
        b = next;
        ++h;
        if (b > (seq_t)nmax) break;   /* first height with min leaves > nmax */
        if (h + 1 >= minL_size) {     /* grow table if needed */
            minL_size *= 2;
            minL = (idx_t *)realloc(minL, minL_size * sizeof(idx_t));
        }
        minL[h] = (idx_t)b;            /* store F_{h+2} */
    }
    /*  h is one past the last needed height; subtract 1  */
    dp_height = h - 1;
    minL_size = dp_height + 1;

    /*  allocate DP arrays  */
    dp = (seq_t **)malloc((dp_height + 1) * sizeof(seq_t *));
    for (size_t height = 0; height <= dp_height; ++height) {
        dp[height] = (seq_t *)calloc((size_t)nmax + 1, sizeof(seq_t));
    }
    dp_nmax = (size_t)nmax;

    /*  base case: height 0, 1 leaf  */
    dp[0][1] = (seq_t)1;

    /*  compute DP for all heights and leaf counts  */
    for (size_t height = 1; height <= dp_height; ++height) {
        idx_t minLeaves_h   = minL[height];
        idx_t minLeaves_h1  = minL[height - 1];
        idx_t minLeaves_h2  = (height >= 2) ? minL[height - 2] : 0;

        for (idx_t n = minLeaves_h; n <= nmax; ++n) {
            seq_t sum = (seq_t)0;

            /*  both children height = height-1  */
            if (minLeaves_h1 <= n / 2) {
                idx_t l_start = minLeaves_h1;
                idx_t l_end   = n - minLeaves_h1;
                if (l_end >= l_start) {
                    for (idx_t l = l_start; l <= l_end; ++l) {
                        sum += dp[height - 1][(size_t)l] * dp[height - 1][(size_t)(n - l)];
                    }
                }
            }

            if (height >= 2) {
                /*  left height = height-1, right = height-2  */
                if (minLeaves_h1 <= n - minLeaves_h2) {
                    idx_t l_start = minLeaves_h1;
                    idx_t l_end   = n - minLeaves_h2;
                    if (l_end >= l_start) {
                        for (idx_t l = l_start; l <= l_end; ++l) {
                            sum += dp[height - 1][(size_t)l] * dp[height - 2][(size_t)(n - l)];
                        }
                    }
                }
                /*  left height = height-2, right = height-1  */
                if (minLeaves_h2 <= n - minLeaves_h1) {
                    idx_t l_start = minLeaves_h2;
                    idx_t l_end   = n - minLeaves_h1;
                    if (l_end >= l_start) {
                        for (idx_t l = l_start; l <= l_end; ++l) {
                            sum += dp[height - 2][(size_t)l] * dp[height - 1][(size_t)(n - l)];
                        }
                    }
                }
            }

            dp[height][(size_t)n] = sum;
        }
    }
}

/*  public interface  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    if ((size_t)n > dp_nmax) {
        recompute_dp(n);      /* compute tables up to required n */
    }

    /*  find the first height with a non‑zero count  */
    for (size_t height = 0; height <= dp_height; ++height) {
        if (dp[height][(size_t)n] != (seq_t)0) {
            return dp[height][(size_t)n];
        }
    }

    /*  should not happen for n >= 1  */
    return (seq_t)0;
}
