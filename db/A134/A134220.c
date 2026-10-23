/*
 * A134220  Values n for which integer solutions of Mordell curve y^2=x^3+n
 *          have a single value of x (or equivalently of nonnegative y).
 * https://oeis.org/A134220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth smallest integer n such that the Mordell curve
 *            y² = x³ + n has exactly one integer solution in x (equivalently,
 *            one non‑negative y)
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   int_cuberoot_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---------------  UTILITIES  ---------------------------------- */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/int_cuberoot_u128.h"

/* integer square root for 64‑bit unsigned integers (floor) */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = (uint64_t)1 << 32;
    while (lo <= hi)
    {
        uint64_t mid = (lo + hi) >> 1;
        uint64_t sq = mid * mid;
        if (sq == n) return mid;
        else if (sq < n) lo = mid + 1;
        else hi = mid - 1;
    }
    return hi;
}

/* ---------------  SEQUENCE GENERATION  ------------------------ */

/* global state */
static seq_t *g_terms     = NULL;   /* array of sequence values */
static size_t g_terms_len = 0;      /* number of stored terms   */
static size_t g_terms_cap = 0;      /* capacity of terms array */
static uint8_t *g_counts   = NULL;   /* array counting solutions */
static size_t g_counts_cap = 0;     /* capacity of counts array */

static const size_t N_LIMIT = 1000000ULL;   /* maximum n to generate */

/* expand terms array if needed */
static void ensure_terms_cap(size_t needed)
{
    if (needed <= g_terms_cap) return;
    size_t new_cap = g_terms_cap ? g_terms_cap : 128;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(g_terms, new_cap * sizeof(seq_t));
    if (!new_arr) abort();
    g_terms = new_arr;
    g_terms_cap = new_cap;
}

/* initialize the sequence data structures */
static void init_sequence(void)
{
    /* allocate count array for n = 0 .. N_LIMIT */
    size_t count_cap = N_LIMIT + 1;
    g_counts = (uint8_t *)calloc(count_cap, sizeof(uint8_t));
    if (!g_counts) abort();

    g_counts_cap = count_cap;

    /* compute upper bound for |x| needed to cover all solutions
       for n <= N_LIMIT.  It suffices to use (N_LIMIT^2)^{1/3} + 5.   */
    unsigned __int128 prod = (unsigned __int128)N_LIMIT * N_LIMIT;
    size_t x_max = int_cuberoot_u128(prod) + 5;

    /* iterate over all integer x in [-x_max , x_max] */
    for (int64_t x = -(int64_t)x_max; x <= (int64_t)x_max; ++x)
    {
        int64_t x_cube = x * x * x;  /* signed 64‑bit cube */

        if (x_cube >= 0)
        {
            /* n = y² - x³, 1 ≤ n ≤ N_LIMIT  ->  y² in [x³+1 , x³+N_LIMIT] */
            uint64_t low  = (uint64_t)x_cube + 1ULL;
            uint64_t high = low + N_LIMIT;

            uint64_t y_start = isqrt_u64(low);
            if (y_start * y_start < low) ++y_start;   /* ceil sqrt */

            uint64_t y_end = isqrt_u64(high);
            if (y_start > y_end) continue;

            for (uint64_t y = y_start; y <= y_end; ++y)
            {
                uint64_t y2 = y * y;
                int64_t n_val = (int64_t)(y2) - x_cube;   /* n = y² - x³ */
                if (n_val >= 1 && (uint64_t)n_val <= N_LIMIT)
                {
                    uint8_t cn = g_counts[(size_t)n_val];
                    if (cn < 3) g_counts[(size_t)n_val] = cn + 1;
                }
            }
        }
        else   /* x_cube < 0 */
        {
            uint64_t M = (uint64_t)(-x_cube);   /* -x³  > 0   */
            if (M > N_LIMIT) continue;          /* n would be > N_LIMIT */

            uint64_t max_y2 = N_LIMIT - M;      /* y² ≤ N_LIMIT – M   */
            uint64_t y_end = isqrt_u64(max_y2); /* y from 0 to y_end  */
            for (uint64_t y = 0; y <= y_end; ++y)
            {
                uint64_t y2   = y * y;
                uint64_t n_val = y2 + M;        /* n = y² + M */
                if (n_val >= 1 && n_val <= N_LIMIT)
                {
                    uint8_t cn = g_counts[(size_t)n_val];
                    if (cn < 3) g_counts[(size_t)n_val] = cn + 1;
                }
            }
        }
    }

    /* collect all n with exactly one solution into sorted array */
    for (size_t n = 1; n <= N_LIMIT; ++n)
    {
        if (g_counts[n] == 1)
        {
            ensure_terms_cap(g_terms_len + 1);
            g_terms[g_terms_len++] = (seq_t)n;
        }
    }

    /* free count array; data needed only for initial construction */
    free(g_counts);
    g_counts = NULL;
    g_counts_cap = 0;
}

/* --------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* initialise once */
    static bool init_done = false;
    if (!init_done)
    {
        init_sequence();
        init_done = true;
    }

    /* n is 1‑based index */
    if (n == 0 || (size_t)n > g_terms_len) return (seq_t)0;
    return g_terms[(size_t)n - 1];
}
