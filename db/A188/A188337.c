/*
 * A188337  Number of nondecreasing arrangements of 7 nonzero numbers in
 *          -(n+5)..(n+5) with sum zero.
 * https://oeis.org/A188337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 86, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Empirical recurrence for A188337 (order 22):
 * a(n) = 2*a(n-1) - a(n-3) - a(n-5) + a(n-6) - 2*a(n-7) + 2*a(n-8) + a(n-9)
 *        - a(n-13) - 2*a(n-14) + 2*a(n-15) - a(n-16) + a(n-17) + a(n-19)
 *        - 2*a(n-21) + a(n-22)
 * The first 22 terms are computed by brute force (search of multisets of size 7).
 *
 * This function is memoised and works for any call order.
 */
#include <stdlib.h>
#include <stdint.h>

/* Brute‑force calculation for n ≤ 22.
 * Count non‑decreasing 7‑tuples of non‑zero integers in [-(n+5)..-(1)]∪[1..(n+5)] whose sum is 0. */
static seq_t brute_a(idx_t n)
{
    idx_t m = n + 5;                /* absolute value bound */
    int range = 14 * (int)m + 1;    /* sums from -7m to +7m inclusive */
    int offset = 7 * (int)m;

    /* DP table: rows for used numbers (0..7), columns for sum index (0..range-1) */
    seq_t *dp = (seq_t *)calloc(8 * range, sizeof(seq_t));
    if (!dp) exit(1);
    dp[0 * range + offset] = 1;

    /* Process negative values first, then positive values. */
    for (idx_t i = 0; i < m; ++i) {           /* v = -(i+1) */
        int v = -(int)(i + 1);
        for (int used = 0; used <= 6; ++used) {
            seq_t *row = dp + used * range;
            seq_t *nextRow = dp + (used + 1) * range;
            for (int s = 0; s < range; ++s) {
                seq_t cnt = row[s];
                if (!cnt) continue;
                int ns = s + v;
                if (ns >= 0 && ns < range)
                    nextRow[ns] += cnt;
            }
        }
    }

    for (idx_t i = 1; i <= m; ++i) {           /* v = i */
        int v = (int)i;
        for (int used = 0; used <= 6; ++used) {
            seq_t *row = dp + used * range;
            seq_t *nextRow = dp + (used + 1) * range;
            for (int s = 0; s < range; ++s) {
                seq_t cnt = row[s];
                if (!cnt) continue;
                int ns = s + v;
                if (ns >= 0 && ns < range)
                    nextRow[ns] += cnt;
            }
        }
    }

    seq_t result = dp[7 * range + offset];
    free(dp);
    return result;
}

/* Cache for memoisation */
static seq_t *s_cache = NULL;
static idx_t s_size = 0;   /* currently cached terms a(1) … a(s_size) */
static idx_t s_cap  = 0;   /* allocated capacity */

/* Ensure cache can hold at least `n` terms. */
static void ensure_capacity(idx_t n)
{
    if (n <= s_cap) return;
    idx_t new_cap = s_cap ? s_cap * 2 : 64;
    if (new_cap < n) new_cap = n;
    seq_t *new_cache = (seq_t *)realloc(s_cache, sizeof(seq_t) * new_cap);
    if (!new_cache) exit(1);
    s_cache = new_cache;
    s_cap   = new_cap;
}

/* Public function requested by the harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, but keep defined */

    ensure_capacity(n);

    /* Compute first 22 terms by brute force if needed. */
    if (s_size < 22) {
        idx_t upto = n < 22 ? n : 22;
        for (idx_t i = s_size + 1; i <= upto; ++i)
            s_cache[i - 1] = brute_a(i);
        s_size = upto;
    }

    if (n <= s_size) return s_cache[n - 1];

    /* Use the empirical recurrence for n > 22. */
    for (idx_t i = s_size + 1; i <= n; ++i) {
        seq_t val = 0;
        val += s_cache[i - 2] * 2;                 /* 2*a(i-1) */
        val -= s_cache[i - 4];                     /* -a(i-3)    */
        val -= s_cache[i - 6];                     /* -a(i-5)    */
        val += s_cache[i - 7];                     /* +a(i-6)    */
        val -= s_cache[i - 8] * 2;                 /* -2*a(i-7)  */
        val += s_cache[i - 9] * 2;                 /* +2*a(i-8)  */
        val += s_cache[i -10];                     /* +a(i-9)    */
        val -= s_cache[i -14];                     /* -a(i-13)   */
        val -= s_cache[i -15] * 2;                 /* -2*a(i-14) */
        val += s_cache[i -16] * 2;                 /* +2*a(i-15) */
        val -= s_cache[i -17];                     /* -a(i-16)   */
        val += s_cache[i -18];                     /* +a(i-17)   */
        val += s_cache[i -20];                     /* +a(i-19)   */
        val -= s_cache[i -22] * 2;                 /* -2*a(i-21) */
        val += s_cache[i -23];                     /* +a(i-22)   */
        s_cache[i - 1] = val;
    }
    s_size = n;
    return s_cache[n - 1];
}
