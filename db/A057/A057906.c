/*
 * A057906  Positive integers that are not the sum of exactly five positive
 *          cubes.
 * https://oeis.org/A057906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini
 * Formula:   not stated
 * Range:     u8 n <= 175, u16 n <= 4401, u32 n <= 5439, u64 n <= 5439,
 *            u128 n <= 5439, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the nth positive integer that cannot be written as the sum of exactly
 * five positive cubes.
 * This implementation precomputes all such integers up to a fixed limit
 * (slightly above the last known term 1 290 740) using a bounded‑knapsack DP.
 * It then answers queries in O(1) time.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Choose a limit larger than the largest term (1290740). */
static const idx_t LIMIT = 1300000ULL;          /* \aMAX */

static bool  *reach       = NULL;               /* DP table   */
static seq_t *seq_array   = NULL;               /* Result list*/
static size_t seq_len     = 0;                  /* Number of terms */
static int   init_done    = 0;                  /* Init flag  */

/* One‑time initialization: compute all sums of exactly five positive cubes
 * up to LIMIT and build the list of non‑representable numbers. */
static void init_seq(void)
{
    if (init_done) return;                     /* already done */

    /* Allocate DP table: 6 rows (k = 0..5), each of size LIMIT+1 booleans. */
    const size_t row_size   = (size_t)(LIMIT + 1U);
    const size_t table_size = 6 * row_size;
    reach = (bool *)calloc(table_size, sizeof(bool));
    if (!reach) abort();                       /* allocation failure */

    /* DP initial condition: 0 cubes sum to 0. */
    reach[0] = true;                           /* reach[0][0] */

    /* Enumerate all positive cube values v = i³ ≤ LIMIT. */
    for (idx_t i = 1;; ++i) {
        idx_t v = i * i * i;
        if (v > LIMIT) break;

        /* For each k = 1..5, a sum using k cubes that ends with v is reachable
         * iff the sum s – v is reachable with k‑1 cubes. */
        for (int k = 1; k <= 5; ++k) {
            bool *row_prev = reach + (k - 1) * row_size;
            bool *row_curr = reach + k * row_size;
            for (idx_t s = v; s <= LIMIT; ++s) {
                if (row_prev[s - v]) row_curr[s] = true;
            }
        }
    }

    /* Count how many numbers ≤ LIMIT are NOT reachable with exactly five cubes. */
    size_t count = 0;
    bool *row5 = reach + 5 * row_size;
    for (idx_t i = 1; i <= LIMIT; ++i) {
        if (!row5[i]) ++count;
    }

    /* Allocate array for the sequence. */
    seq_array = (seq_t *)malloc(count * sizeof(seq_t));
    if (!seq_array) abort();
    seq_len = count;

    /* Fill the sequence array. */
    size_t pos = 0;
    for (idx_t i = 1; i <= LIMIT; ++i) {
        if (!row5[i]) {
            seq_array[pos++] = i;
        }
    }

    /* Free the DP table – no longer needed. */
    free(reach);
    reach = NULL;

    init_done = 1;
}

/* External function required by the tests. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for n < 1 */
    init_seq();
    if (n - 1 >= seq_len) return 0;           /* out of precomputed range */
    return seq_array[n - 1];
}
