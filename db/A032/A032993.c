/*
 * A032993  If d,e are consecutive digits of n in base 11, then |d-e|>=4.
 * https://oeis.org/A032993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A032993 – numbers in base 11 whose consecutive digits differ by ≥ 4 */
#include <stdint.h>
#include <stdbool.h>

/* Enumerate the words over the alphabet {0,…,10} with the adjacency rule
   |d−e| ≥ 4.  For a given rank n the algorithm reconstructs the n‑th word
   in lexicographic order and returns its value in base 11.  The implementation
   works for any unsigned integer type seq_t and idx_t provided by the build,
   with a maximum word length of 10 which is sufficient for all n ≤ 100 000
   (and in particular for the 10 000 limit requested).  */

/* Memoized data structures – all sizes are compile‑time constants. */
#define MAXLEN 10        /* maximum word length needed for the tested range */

static bool          init_done = false;
static int           allowed[11][11];     /* adjacency lists, ascending order */
static int           allowed_cnt[11];
static idx_t         dp[MAXLEN + 1][11];   /* dp[rem][prev] = ways to finish rem positions */

static void init(void)
{
    int i, j;
    /* Build adjacency lists */
    for (i = 0; i <= 10; ++i) {
        int cnt = 0;
        for (j = 0; j <= 10; ++j) {
            if (i >= j + 4 || j >= i + 4) {
                allowed[i][cnt++] = j;
            }
        }
        allowed_cnt[i] = cnt;
    }
    /* DP for remaining positions: dp[0][*] = 1 (nothing to place) */
    for (i = 0; i <= 10; ++i) dp[0][i] = 1;
    for (i = 1; i <= MAXLEN; ++i) {
        for (j = 0; j <= 10; ++j) {
            idx_t s = 0;
            for (int k = 0; k < allowed_cnt[j]; ++k) {
                unsigned int nxt = allowed[j][k];
                s += dp[i - 1][nxt];
            }
            dp[i][j] = s;
        }
    }
    init_done = true;
}

seq_t a(idx_t n)
{
    if (!init_done) init();
    idx_t cumulative = 0;
    int  len = 0;
    idx_t len_count = 0;
    /* Find the length of the n‑th number */
    for (len = 1; len <= MAXLEN; ++len) {
        if (len == 1) {
            len_count = 10;          /* digits 1 … 10 for the first position */
        } else {
            len_count = 0;
            for (int d = 1; d <= 10; ++d)        /* first digit cannot be 0 */
                len_count += dp[len - 1][d];
        }
        if (cumulative + len_count >= n) break;
        cumulative += len_count;
    }
    /* Rank within numbers of the found length */
    idx_t k = n - cumulative;   /* 1‑based rank */

    seq_t value = 0;
    int   prev = -1;            /* sentinel for the first position */

    for (int pos = 0; pos < len; ++pos) {
        int rem = len - 1 - pos;      /* remaining positions after this one */
        int chosen = -1;

        if (pos == 0) {
            /* First digit choices: 1 … 10 */
            for (int d = 1; d <= 10; ++d) {
                idx_t cnt = dp[rem][d];
                if (cnt < k) {
                    k -= cnt;
                } else {
                    chosen = d;
                    break;
                }
            }
        } else {
            /* Follow the adjacency rule */
            for (int idx = 0; idx < allowed_cnt[prev]; ++idx) {
                int d = allowed[prev][idx];
                idx_t cnt = dp[rem][d];
                if (cnt < k) {
                    k -= cnt;
                } else {
                    chosen = d;
                    break;
                }
            }
        }

        /* Append the chosen digit to the number (base 11). */
        value = value * ((seq_t)11) + ((seq_t)chosen);
        prev = chosen;
    }

    return value;
}
