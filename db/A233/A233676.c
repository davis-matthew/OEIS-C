/*
 * A233676  Number of (n+1)X(2+1) 0..3 arrays with every 2X2 subblock having
 *          the sum of the squares of the edge differences equal to 6.
 * https://oeis.org/A233676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 24, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A233676
   Number of (n+1)×3 arrays with entries 0..3 such that each 2×2 subblock
   has the sum of the squares of the edge differences equal to 6.
   Computation is performed by dynamic programming over all 64 possible
   rows.  The adjacency matrix A[i][j] is 1 iff rows i→j satisfy the
   constraint for both 2×2 blocks.  a(n) equals the sum of all walk
   counts of length n in this directed graph, i.e. the sum of the
   entries of (A^n). */

/* The routine assumes that seq_t and idx_t are defined elsewhere. */
#include <stdbool.h>
#include <stdlib.h>   /* for malloc and realloc */

static bool adjacency[64][64] = {0};
static bool adj_initialized = false;

/* All 64 rows as arrays of 3 base‑4 digits */
static uint8_t row_digits[64][3];

/* Convert a state 0..63 to its 3 digits (row representation) */
static void init_row_digits(void)
{
    for (idx_t s = 0; s < 64; ++s) {
        row_digits[s][0] = (s / 16) % 4;
        row_digits[s][1] = (s /  4) % 4;
        row_digits[s][2] =  s % 4;
    }
}

/* Check the 2×2 block constraint for a pair of rows. Returns true
   iff both blocks (columns 0‑1 and 1‑2) satisfy the condition. */
static bool block_ok(const uint8_t a[3], const uint8_t b[3])
{
    for (int k = 0; k < 2; ++k) {
        int tl = a[k], tr = a[k+1], bl = b[k], br = b[k+1];
        int s = (tl - tr)*(tl - tr)       // top horizontal
              + (bl - br)*(bl - br)       // bottom horizontal
              + (tl - bl)*(tl - bl)       // left vertical
              + (tr - br)*(tr - br);      // right vertical
        if (s != 6) return false;
    }
    return true;
}

/* Precompute adjacency matrix and row digits if not yet done. */
static void init_adjacency(void)
{
    if (adj_initialized) return;
    init_row_digits();
    for (idx_t i = 0; i < 64; ++i) {
        for (idx_t j = 0; j < 64; ++j) {
            adjacency[i][j] = block_ok(row_digits[i], row_digits[j]);
        }
    }
    adj_initialized = true;
}

/* DP state: counts for arrays ending in each row at the current step. */
static seq_t curr_counts[64];

/* Cached values of a(n) for 1 ≤ n ≤ cache_len.  Index 1 stores a(1). */
static seq_t *cache = NULL;
static idx_t cache_len = 0;       /* number of stored values (max n computed) */
static idx_t cache_cap = 0;       /* current capacity of cache array */

/* Ensure the cache contains values up to n. */
static void ensure_cache(idx_t n)
{
    if (!adj_initialized) init_adjacency();

    /* Grow the cache array if necessary. */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * (cache_cap + 1));
    }
    while (cache_cap < n) {
        cache_cap *= 2;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * (cache_cap + 1));
    }

    /* Initialize DP counts for step 0 (arrays of height 1). */
    if (cache_len == 0) {
        for (idx_t i = 0; i < 64; ++i) curr_counts[i] = (seq_t)1;
    }

    /* Compute successive steps until reaching n. */
    for (idx_t step = cache_len; step < n; ++step) {
        seq_t next_counts[64];
        /* zero next_counts */
        for (idx_t j = 0; j < 64; ++j) next_counts[j] = (seq_t)0;

        /* Transition: for each current row i, add its count to all allowed next rows j. */
        for (idx_t i = 0; i < 64; ++i) {
            seq_t c = curr_counts[i];
            if (c == 0) continue;
            for (idx_t j = 0; j < 64; ++j) {
                if (adjacency[i][j]) next_counts[j] += c;
            }
        }

        /* Copy back to curr_counts and compute a(step+1). */
        seq_t total = (seq_t)0;
        for (idx_t j = 0; j < 64; ++j) {
            curr_counts[j] = next_counts[j];
            total += next_counts[j];
        }

        /* Store result: step+1 corresponds to a(step+1). */
        cache[step + 1] = total;
    }
    cache_len = n;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* a(0) is undefined; return 0 */
    ensure_cache(n);
    return cache[n];
}
