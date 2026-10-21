/*
 * A206983  Number of nX4 0..1 arrays avoiding the patterns 0 1 0 or 1 0 1 in
 *          any row, column, diagonal or antidiagonal.
 * https://oeis.org/A206983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 36, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Seq. A206983: Number of n×4 0/1 arrays avoiding the patterns
   010 or 101 in any row, column, diagonal or antidiagonal.
   The sequence satisfies a(1)=10, a(2)=100, a(3)=158, a(4)=556, a(5)=1866,
   a(6)=5804, a(7)=18528, a(8)=59888, a(9)=191484, a(10)=612874, … .
   A dynamic‑programming algorithm is used; it is linear in n. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/* Parameters                                                        */
/* ------------------------------------------------------------------ */
#define NROWS 10                     /* number of rows with no horizontal 010/101  */
#define STEM  4                      /* column width                                 */
#define STATES (NROWS * NROWS)       /* pair of consecutive rows                     */
#define MAXTRANS NROWS               /* max transitions from one state (≤NROWS)     */

/* ------------------------------------------------------------------ */
/* Valid row values                                                  */
/* ------------------------------------------------------------------ */
static const unsigned char valid_row_vals[NROWS] = {
    0x0, 0x1, 0x3, 0x6, 0x7,
    0x8, 0x9, 0xC, 0xE, 0xF
};

/* ------------------------------------------------------------------ */
/* Transition tables                                                 */
/* ------------------------------------------------------------------ */
static unsigned char next_state[STATES][MAXTRANS];
static unsigned char next_count[STATES];
static bool tables_built = false;

/* ------------------------------------------------------------------ */
/* Utility: check whether a 4‑bit row contains 010 or 101 horizontally */
static bool is_valid_row(unsigned char r)
{
    for (int j = 0; j <= 1; ++j) {
        unsigned char bits = (r >> j) & 0x7;   /* 3 consecutive bits */
        if (bits == 0x2 || bits == 0x5)        /* 010 or 101 */
            return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Utility: check whether adding a new row keeps all vertical,
   main‑diagonal, and anti‑diagonal windows free of 010/101.  */
static bool valid_transition(unsigned char a, unsigned char b,
                             unsigned char r)
{
    /* vertical patterns */
    for (int col = 0; col < STEM; ++col) {
        unsigned char bits =
            ((a >> col) & 1) << 2 |
            ((b >> col) & 1) << 1 |
            ((r >> col) & 1);
        if (bits == 0x2 || bits == 0x5)
            return false;
    }
    /* main diagonal: positions (a,col-1)-(b,col)-(r,col+1) */
    for (int col = 1; col <= 2; ++col) {
        unsigned char bits =
            ((a >> (col - 1)) & 1) << 2 |
            ((b >> col) & 1) << 1 |
            ((r >> (col + 1)) & 1);
        if (bits == 0x2 || bits == 0x5)
            return false;
    }
    /* anti diagonal: positions (a,col+1)-(b,col)-(r,col-1) */
    for (int col = 1; col <= 2; ++col) {
        unsigned char bits =
            ((a >> (col + 1)) & 1) << 2 |
            ((b >> col) & 1) << 1 |
            ((r >> (col - 1)) & 1);
        if (bits == 0x2 || bits == 0x5)
            return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Build the transition tables once.                                  */
/* ------------------------------------------------------------------ */
static void build_transition_tables(void)
{
    for (unsigned char a_i = 0; a_i < NROWS; ++a_i) {
        unsigned char a = valid_row_vals[a_i];
        for (unsigned char b_i = 0; b_i < NROWS; ++b_i) {
            unsigned char b = valid_row_vals[b_i];
            unsigned char state = a_i * NROWS + b_i;
            unsigned char cnt = 0;
            for (unsigned char c_i = 0; c_i < NROWS; ++c_i) {
                unsigned char r = valid_row_vals[c_i];
                if (valid_transition(a, b, r)) {
                    next_state[state][cnt] = b_i * NROWS + c_i;
                    ++cnt;
                }
            }
            next_count[state] = cnt;
        }
    }
}

/* ------------------------------------------------------------------ */
/* DP vectors: dp_cur holds momentary counts for last two rows,
   dp_next will accumulate the next frontier.                         */
/* ------------------------------------------------------------------ */
static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;

/* ------------------------------------------------------------------ */
/* Cache of computed a(n) values                                       */
/* ------------------------------------------------------------------ */
static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* number of cached terms (>=1 if any term cached) */

/* ------------------------------------------------------------------ */
/* Sum all entries of a DP vector.                                     */
/* ------------------------------------------------------------------ */
static seq_t sum_dp(seq_t *vec)
{
    seq_t total = 0;
    for (unsigned char s = 0; s < STATES; ++s)
        total += vec[s];
    return total;
}

/* ------------------------------------------------------------------ */
/* Public function                                                   */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;     /* undefined for n=0 as per offset */

    /* Prepare tables and DP vectors on first use. */
    if (!tables_built) {
        build_transition_tables();
        tables_built = true;
    }
    if (!dp_cur) {
        dp_cur  = (seq_t *)calloc(STATES, sizeof(seq_t));
        dp_next = (seq_t *)calloc(STATES, sizeof(seq_t));
        /* all 10×10 pair states are initially valid for n=2 */
        for (unsigned char s = 0; s < STATES; ++s)
            dp_cur[s] = 1;
    }

    /* Ensure cache large enough. */
    size_t needed = (n > cache_len) ? n : cache_len;
    seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!new_cache && needed > 0)
        return 0;    /* allocation failed; return 0 */
    cache = new_cache;

    /* Initialise cache[0] if this is the first lookup. */
    if (cache_len == 0) {
        cache[0] = 10;     /* a(1) */
        cache_len = 1;
    }

    /* If a(2) has not been computed yet, compute it now. */
    if (cache_len < 2 && n >= 2) {
        cache[1] = sum_dp(dp_cur);   /* sum of all pair states at length 2 */
        cache_len = 2;
    }

    /* Compute terms from current cache_len + 1 up to n. */
    for (idx_t cur = cache_len + 1; cur <= n; ++cur) {
        if (cur == 2) {
            /* Already handled above; skip. */
            continue;
        }
        /* Transition from dp_cur (length cur-1) to dp_next (length cur). */
        for (unsigned char s = 0; s < STATES; ++s) {
            seq_t val = dp_cur[s];
            if (val == 0)
                continue;
            unsigned char cnt = next_count[s];
            for (unsigned char k = 0; k < cnt; ++k) {
                unsigned char ns = next_state[s][k];
                dp_next[ns] += val;
            }
        }
        /* Store the result for length cur. */
        cache[cur - 1] = sum_dp(dp_next);

        /* Prepare for the next iteration. */
        memset(dp_cur, 0, STATES * sizeof(seq_t));
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    cache_len = n;   /* we now have all terms up to n cached */
    return cache[n - 1];
}
