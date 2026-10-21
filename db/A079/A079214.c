/*
 * A079214  Catalan paths: numbers starting with 1 and ending with 0 where
 *          each digit is nonnegative and adjacent digits differ by 1.
 * https://oeis.org/A079214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A079214: Catalan paths
   Numbers starting with 1 and ending with 0 where each digit is
   nonnegative (0‑9) and adjacent digits differ by ±1.
   The terms are listed in increasing numeric order.
   This function returns the nth term (n ≥ 1).  It uses
   lexical enumeration with a DP that respects the upper digit
   bound of 9.  The implementation is valid for any seq_t
   and idx_t supplied by the build. */

#include <stdint.h>
#include <stdbool.h>

/* maximum path length for n ≤ 10000 (k ≤ 10, length 20) – keep room */
#define MAX_K      15                   /* enough for n up to 10000 */
#define MAX_STEPS (2 * MAX_K - 1)       /* maximum steps after first digit */

/* pre‑computed tables, populated once */
static idx_t cat[MAX_K + 1];
static idx_t cum[MAX_K + 1];
static idx_t dp[10][MAX_STEPS + 1];    /* dp[h][s] = #ways from h to 0 in s steps */
static bool   tables_ready = false;

/* initialise tables; called once lazily */
static void init_tables(void)
{
    if (tables_ready) return;
    tables_ready = true;

    /* DP for walks with height in [0,9] */
    for (int h = 0; h <= 9; ++h)
        dp[h][0] = (h == 0 ? (idx_t)1 : (idx_t)0);

    for (int s = 1; s <= MAX_STEPS; ++s) {
        for (int h = 0; h <= 9; ++h) {
            idx_t up   = (h < 9) ? dp[h + 1][s - 1] : (idx_t)0;
            idx_t down = (h > 0) ? dp[h - 1][s - 1] : (idx_t)0;
            dp[h][s] = up + down;
        }
    }

    /* Catalan numbers C_0 … C_MAX_K */
    cat[0] = (idx_t)1;
    for (int i = 1; i <= MAX_K; ++i) {
        idx_t sum = 0;
        for (int j = 0; j < i; ++j)
            sum += cat[j] * cat[i - 1 - j];
        cat[i] = sum;
    }

    /* cumulative sums of Catalan blocks */
    cum[0] = (idx_t)0;
    for (int i = 1; i <= MAX_K; ++i)
        cum[i] = cum[i - 1] + cat[i];
}

/* the required function */
seq_t a(idx_t n)
{
    init_tables();

    if (n == 0) return (seq_t)0;          /* undefined for n < 1 */

    /* find block length 2*k */
    idx_t k = 1;
    while (k <= MAX_K && n > cum[k]) ++k;
    if (k > MAX_K) return (seq_t)0;       /* safety – should not happen */

    /* offset within this block (0‑based) */
    idx_t offset = n - cum[k - 1] - (idx_t)1;
    unsigned int len = 2 * (unsigned int)k;   /* number of digits */

    /* array to hold the digits of the number */
    int digits[(2 * MAX_K)] = {0};
    digits[0] = 1;
    int h = 1;  /* current height */

    for (unsigned int pos = 1; pos < len; ++pos) {
        unsigned int rem = len - pos - 1;            /* remaining steps */

        /* possible next heights */
        int low  = (h > 0) ? h - 1 : -1;
        int high = (h < 9) ? h + 1 : -1;

        /* iterate candidates in increasing order (lexicographic) */
        int candidates[2] = {low, high};
        for (int ci = 0; ci < 2; ++ci) {
            int cand = candidates[ci];
            if (cand < 0) continue;                 /* not a valid move */

            idx_t ways = dp[cand][rem];
            if (offset < ways) {
                digits[pos] = cand;
                h = cand;
                break;
            }
            offset -= ways;
        }
    }

    /* convert digit array to a integer in seq_t */
    seq_t val = (seq_t)digits[0];
    for (unsigned int i = 1; i < len; ++i)
        val = val * (seq_t)10 + (seq_t)digits[i];

    return val;
}
