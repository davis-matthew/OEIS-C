/*
 * A043841  Number of runs in the base 4 representation of n is congruent to 4
 *          mod 6.
 * https://oeis.org/A043841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the nth integer whose base‑4 representation has a number of runs
 * congruent to 4 (mod 6).  The function lazily generates the sequence
 * as it is needed, caching previously found terms. */
#include <stdlib.h>

/* Count number of runs in the base‑4 representation of n.
 * A run is a maximal consecutive block of equal digits. */
static inline int runs_in_base4(uint64_t n)
{
    if (n == 0) return 1;   /* only digit 0 -> one run */
    int runs = 0;
    int prev = -1;
    while (n) {
        int d = n & 3;          /* n % 4 */
        if (d != prev) {
            runs++;
            prev = d;
        }
        n >>= 2;                /* n /= 4 */
    }
    return runs;
}

static uint64_t *vals = NULL;        /* cached integers satisfying the property */
static size_t vals_len = 0;          /* number of cached integers */
static size_t vals_cap = 0;          /* allocated capacity of vals */
static uint64_t next_num = 0;        /* next integer to test */

/* Ensure that at least 'need' terms have been generated and stored. */
static void ensure_terms(size_t need)
{
    if (vals_len >= need) return;

    /* Grow the array as needed */
    if (vals_cap == 0) {
        vals_cap = 16;
        vals = (uint64_t*)malloc(vals_cap * sizeof(uint64_t));
    }
    while (vals_len < need) {
        /* Find next integer that satisfies the property */
        do { ++next_num; } while (runs_in_base4(next_num) % 6 != 4);

        /* Store it */
        if (vals_len + 1 > vals_cap) {
            vals_cap *= 2;
            vals = (uint64_t*)realloc(vals, vals_cap * sizeof(uint64_t));
        }
        vals[vals_len++] = next_num;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined by OEIS offset, return 0 */
    ensure_terms((size_t)n);
    return (seq_t)vals[n - 1];
}
