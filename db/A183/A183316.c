/*
 * A183316  Number of nX4 binary arrays with an element zero only if there are
 *          an even number of ones to its left and an even number of ones
 *          above it.
 * https://oeis.org/A183316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A183316: number of n×4 binary arrays in which a zero occurs
 * only if there are an even number of ones to its left and an even
 * number of ones above it.  Implementation uses a breadth‑first
 * dynamic‑programming over column‑parity states (4 bits).  No direct
 * use of the external utilities is required. */

#include <stdlib.h>
#include <stdbool.h>

/* Transition counts: trans[s][t] = number of row patterns that
 * transform state s into state t.  States are 0..15 (4‑bit parity patterns). */
static unsigned char trans[16][16];
static bool trans_ready = false;

static void init_trans(void)
{
    if (trans_ready) return;
    for (int s = 0; s < 16; ++s) {
        for (int p = 0; p < 16; ++p) {
            bool ok = true;
            int lpar = 0;                     /* parity of ones seen so far in the row */
            for (int col = 0; col < 4; ++col) {
                int bit = (p >> col) & 1;
                if (bit == 0) {
                    if (lpar & 1) { ok = false; break; }   /* left odd -> zero forbidden */
                    if ((s >> col) & 1) { ok = false; break; } /* above odd -> zero forbidden */
                }
                if (bit == 1) lpar ^= 1;
            }
            if (ok) {
                int t = s ^ p;              /* column parities after adding this row */
                trans[s][t] += 1;
            }
        }
    }
    trans_ready = true;
}

/* Cache of computed a(n) values; cache[i] = a(i) for i>=1. */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* DP state distributions for the previous and current rows. */
static seq_t dp_prev[16];
static seq_t dp_curr[16];

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1; return 0 */

    init_trans();

    if (cache_size == 0) {                 /* initialise once */
        for (int i = 0; i < 16; ++i) dp_prev[i] = (seq_t)0;
        dp_prev[0] = (seq_t)1;            /* all column parities even before any rows */
        cache = NULL;
    }

    if (n > cache_size) {
        /* Allocate sufficient space for the cache up to n. */
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;  /* allocation failure */
        cache = new_cache;
        cache[0] = (seq_t)0;              /* unused but initialise for safety */

        /* Compute terms for rows cache_size+1 … n. */
        for (idx_t r = cache_size + 1; r <= n; ++r) {
            for (int t = 0; t < 16; ++t) dp_curr[t] = (seq_t)0;

            /* Apply transitions from the previous state distribution. */
            for (int s = 0; s < 16; ++s) {
                if (dp_prev[s] == (seq_t)0) continue;
                for (int t = 0; t < 16; ++t) {
                    unsigned char ways = trans[s][t];
                    if (!ways) continue;
                    dp_curr[t] += dp_prev[s] * (seq_t)ways;
                }
            }

            /* Total number of arrays of size r. */
            seq_t total = (seq_t)0;
            for (int t = 0; t < 16; ++t) total += dp_curr[t];
            cache[r] = total;

            /* Prepare for next iteration. */
            for (int t = 0; t < 16; ++t) dp_prev[t] = dp_curr[t];
        }
        cache_size = n;
    }

    return cache[n];
}
