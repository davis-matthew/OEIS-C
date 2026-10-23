/*
 * A048267  Largest integer requiring n fifth powers to sum to it, starting
 *          with n=28.
 * https://oeis.org/A048267
 *
 * Offset:    28    (a(28) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = largest integer requiring n fifth powers to sum to it
 * Range:     u8 none, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* maximum n for which a(n) is positive; for n>37 the sequence is 0 */
#define MAX_N 37
/* search limit – overkill for the known values of the sequence */
#define LIMIT 5000

/* cache for dynamic programming: minimal number of fifth powers needed for each m */
static uint16_t dp[LIMIT + 1];

/* cache for the result a(n) for 0..MAX_N */
static seq_t a_cache[MAX_N + 1];

/* flags whether the tables have been built */
static bool tables_built = false;

/* build the DP tables and the a(n) cache */
static void build_tables(void)
{
    /* initialise dp to a large value */
    for (size_t i = 0; i <= LIMIT; ++i) dp[i] = LIMIT + 1;
    dp[0] = 0;

    /* compute all fifth powers <= LIMIT */
    size_t powers[10];
    size_t pcnt = 0;
    for (idx_t k = 1; k <= LIMIT; ++k) {
        seq_t pk = (seq_t)k * k * k * k * k;   /* k^5 in seq_t */
        if (pk > LIMIT) break;
        powers[pcnt++] = (size_t)pk;
    }

    /* dynamic programming to fill dp */
    for (size_t m = 1; m <= LIMIT; ++m) {
        for (size_t p = 0; p < pcnt; ++p) {
            size_t pow = powers[p];
            if (pow <= m) {
                uint16_t val = dp[m - pow] + 1;
                if (val < dp[m]) dp[m] = val;
            }
        }
    }

    /* initialise the a_cache to 0 */
    for (size_t i = 0; i <= MAX_N; ++i) a_cache[i] = 0;

    /* fill the a_cache with the maximum m for each required number of terms */
    for (size_t m = 1; m <= LIMIT; ++m) {
        size_t need = dp[m];
        if (need >= 28 && need <= MAX_N) {
            if ((seq_t)m > a_cache[need]) a_cache[need] = (seq_t)m;
        }
    }

    tables_built = true;
}

/* public function required by the specification */
seq_t a(idx_t n)
{
    if (!tables_built) build_tables();

    if ((idx_t)MAX_N < n) return (seq_t)0;   /* for n > 37 there is none */
    return a_cache[(size_t)n];
}
