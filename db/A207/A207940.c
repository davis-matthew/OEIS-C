/*
 * A207940  Number of 4Xn 0..1 arrays avoiding 0 0 1 and 1 0 0 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A207940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) -6*a(n-2) -42*a(n-3) +62*a(n-4) +91*a(n-5)
 *            -149*a(n-6) -91*a(n-7) +138*a(n-8) +42*a(n-9) -50*a(n-10)
 *            -7*a(n-11) +6*a(n-12) for n>13 (Initial values a(1)..a(13) given
 *            below.)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Number of allowed columns (4‑bit patterns) */
static bool vertical_ok[16];
static int col_bits[8];
static int bit_to_idx[16] = { -1, };

/* Transition table: trans[i][j][k] is true iff
   columns with indices i, j, k can appear consecutively. */
static bool trans[8][8][8];

/* DP state for current length (last two columns) */
static seq_t dp[8][8];
static seq_t next_arr[8][8];

/* Cache of sequence values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Initialization flag */
static bool initialized = false;

/* Ensure cache can hold at least `n` terms (1‑based index). */
static void
ensure_cache(idx_t n)
{
    if (n <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < (size_t)n)
        new_cap = n;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        abort();
    }
    if (new_cap > cache_cap)
        memset(tmp + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Precompute allowed columns and transition table. */
static void
precompute(void)
{
    int m = 0;
    /* Determine allowed columns (vertical constraints). */
    for (int p = 0; p < 16; ++p) {
        bool ok = true;
        /* Vertically check windows of three consecutive rows:
           rows 0‑1‑2 and 1‑2‑3.
           Row 0 is the top row (most significant bit). */
        for (int r = 0; r <= 1; ++r) {
            int b0 = (p >> (3 - r)) & 1;
            int b1 = (p >> (2 - r)) & 1;
            int b2 = (p >> (1 - r)) & 1;
            if ((b0 == 0 && b1 == 0 && b2 == 1) ||
                (b0 == 1 && b1 == 1 && b2 == 0))
                ok = false;
        }
        vertical_ok[p] = ok;
        bit_to_idx[p] = -1;
        if (ok) {
            bit_to_idx[p] = m;
            col_bits[m++] = p;
        }
    }

    /* Compute transitions. */
    for (int i = 0; i < m; ++i) {
        int a = col_bits[i];
        for (int j = 0; j < m; ++j) {
            int b = col_bits[j];
            for (int k = 0; k < m; ++k) {
                int c = col_bits[k];
                bool good = true;
                for (int r = 0; r < 4; ++r) {
                    int a_bit = (a >> (3 - r)) & 1;
                    int b_bit = (b >> (3 - r)) & 1;
                    int c_bit = (c >> (3 - r)) & 1;
                    if ((a_bit == 0 && b_bit == 0 && c_bit == 1) ||
                        (a_bit == 1 && b_bit == 0 && c_bit == 0)) {
                        good = false;
                        break;
                    }
                }
                trans[i][j][k] = good;
            }
        }
    }
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    if (n < 1)
        return 0;

    if (!initialized) {
        precompute();
        ensure_cache(2);
        int m = 8; /* number of allowed columns */
        cache[0] = (seq_t)m;                         /* a(1) */
        cache[1] = (seq_t)m * (seq_t)m;              /* a(2) */
        cache_len = 2;
        for (int i = 0; i < m; ++i)
            for (int j = 0; j < m; ++j)
                dp[i][j] = (seq_t)1;                /* n = 2 */
        initialized = true;
    }

    if ((size_t)n > cache_cap)
        ensure_cache(n);

    /* Compute cached values up to n if necessary. */
    while (cache_len < (size_t)n) {
        /* dp currently represents strings of length cache_len. */
        memset(next_arr, 0, sizeof(next_arr));
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j) {
                seq_t val = dp[i][j];
                if (!val)
                    continue;
                for (int k = 0; k < 8; ++k)
                    if (trans[i][j][k])
                        next_arr[j][k] += val;
            }
        seq_t total = 0;
        for (int j = 0; j < 8; ++j)
            for (int k = 0; k < 8; ++k)
                total += next_arr[j][k];

        cache_len++;
        cache[cache_len - 1] = total;

        /* Move to next length. */
        memcpy(dp, next_arr, sizeof(dp));
    }

    return cache[n - 1];
}
