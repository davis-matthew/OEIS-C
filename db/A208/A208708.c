/*
 * A208708  Number of nX7 0..1 arrays with new values 0..1 introduced in row
 *          major order and no element equal to more than two of its immediate
 *          leftward or upward or right-upward antidiagonal neighbors.
 * https://oeis.org/A208708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) counts n×7 binary arrays in which
   (1) the element introduced at position (0,0) is forced to 0
       (i.e., the first value is 0 and 1 may appear later),
   (2) during row‑major filling no cell equals all of its existing
       neighbors leftward, upward, and right‑upward antidiagonal.
   The calculation is performed by dynamic programming over rows
   using a 128×128 transition matrix derived from the local rule. */

#include <stdint.h>
#include <stdlib.h>

/* Transition matrix: T[a][b] == 1 iff a 7‑bit pattern a can be followed
   by a 7‑bit pattern b respecting the local constraint. */
static unsigned char Tmat[128][128];

/* Pre‑allocation for sequence values (n up to 10000). */
#define MAX_N 10000
static seq_t *a_cache = NULL;
static idx_t cache_max = 0;

/* DP vectors for current and next row counts. */
static seq_t *prev_vec = NULL;
static seq_t *next_vec = NULL;

/* Build the transition matrix once. */
static void init_matrix(void)
{
    for (int a = 0; a < 128; ++a) {
        for (int b = 0; b < 128; ++b) {
            unsigned char good = 1;
            for (int j = 1; j <= 5; ++j) {          /* middle cells only */
                int b_j        = (b >> j)       & 1;
                int prev_bit   = (b >> (j-1))   & 1;
                int up         = (a >> j)       & 1;
                int diag       = (a >> (j+1))   & 1;
                if (b_j == prev_bit && b_j == up && b_j == diag) {
                    good = 0;
                    break;
                }
            }
            Tmat[a][b] = good;
        }
    }
}

/* Ensure global structures are initialized. */
static void init_globals(void)
{
    if (!a_cache) {
        a_cache = (seq_t*)malloc((MAX_N + 1) * sizeof(seq_t));
        prev_vec = (seq_t*)malloc(128 * sizeof(seq_t));
        next_vec = (seq_t*)malloc(128 * sizeof(seq_t));
        /* first row: first bit (column 0) must be 0, others free */
        for (int a = 0; a < 128; ++a)
            prev_vec[a] = (a & 1) ? 0 : 1;
        a_cache[1] = 64;           /* 2^6 */
        cache_max = 1;
        init_matrix();
    }
}

/* Main function requested by the problem statement. */
seq_t a(idx_t n)
{
    if (n < 1 || n > MAX_N) return 0;   /* out of supported range */
    init_globals();
    if (n <= cache_max) return a_cache[n];

    for (idx_t k = cache_max + 1; k <= n; ++k) {
        for (int b = 0; b < 128; ++b) {
            seq_t sum = 0;
            for (int a = 0; a < 128; ++a)
                if (Tmat[a][b]) sum += prev_vec[a];
            next_vec[b] = sum;
        }
        seq_t total = 0;
        for (int b = 0; b < 128; ++b) total += next_vec[b];
        a_cache[k] = total;
        /* Prepare for next iteration */
        for (int b = 0; b < 128; ++b) prev_vec[b] = next_vec[b];
    }
    cache_max = n;
    return a_cache[n];
}
