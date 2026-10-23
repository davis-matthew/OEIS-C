/*
 * A126789  a(n) is the smallest number such that the product of its digits is
 *          n times the sum of its digits, or 0 if no such number exists.
 * https://oeis.org/A126789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 15, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A126789: a(n) = smallest number whose digit product = n * digit sum; 
   0 if no such number exists.
   Computed by exhaustive search over non‑decreasing digit sequences (1–9) up to length 9. */
#include <stdbool.h>

#define MAX_N      200000      /* table bound: enough for n up to about 2×10⁵            */
#define MAX_DIGITS 9           /* 9^9 is the largest product examined                */

static seq_t best[MAX_N + 1] = {0};
static bool   initialized = false;

/* Depth‑first search over non‑decreasing digit sequences. */
static void dfs(int depth, int min_d, seq_t prod, seq_t sum, seq_t num)
{
    if (depth > 0) {
        if (sum != 0 && prod % sum == 0) {
            seq_t nval   = prod / sum;
            seq_t limit  = (seq_t)MAX_N;
            if (nval <= limit) {
                idx_t idx = (idx_t)nval;
                if (best[idx] == 0 || num < best[idx]) {
                    best[idx] = num;
                }
            }
        }
    }
    if (depth == MAX_DIGITS) return;

    for (int d = min_d; d <= 9; ++d) {
        dfs(depth + 1, d,
            prod * (seq_t)d,
            sum + (seq_t)d,
            num  * (seq_t)10 + (seq_t)d);
    }
}

/* One‑time precomputation of the table. */
static void init_best(void)
{
    if (initialized) return;

    for (int d = 1; d <= 9; ++d) {
        dfs(1, d, (seq_t)d, (seq_t)d, (seq_t)d);
    }
    initialized = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for out‑of‑range call */
    init_best();
    if (n <= MAX_N) return best[n];
    return 0;                      /* unknown for n > MAX_N (not needed for n ≤ 10000) */
}
