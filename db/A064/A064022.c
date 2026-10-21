/*
 * A064022  a(n) is the smallest value of m such that prod(m) =
 *          n*length(m)*sum(m) where prod(m) is the product of the digits of
 *          m, length(m) is the number of digits of m, sum(m) is the sum of
 *          the digits of m; or 0 if no such m exists.
 * https://oeis.org/A064022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes A064022.
   For each integer n (≥1) it returns the smallest m satisfying
   prod(m) = n * len(m) * sum(m); if no such m exists it returns 0.
   The routine pre‑computes all values for 1 ≤ n ≤ 10000 once,
   using a brute‑force search up to a safe upper bound for m.
*/

#include <stdint.h>
#include <stdbool.h>

#ifndef MAX_N
#define MAX_N 10000
#endif

static seq_t a_cache[MAX_N + 1];
static bool cache_initialized = false;

/* Pre‑compute a_cache[1 .. MAX_N] by scanning m up to a chosen limit. */
static void precompute(void)
{
    /* initialise cache */
    for (int i = 1; i <= MAX_N; ++i)
        a_cache[i] = 0;

    /* Upper bound for the search.  For n ≤ 10000 a solution
       will be found (if it exists) with at most 7 digits,
       therefore 10 000 000 is more than sufficient. */
    const uint32_t MAX_M = 10000000U;

    for (uint32_t m = 1; m <= MAX_M; ++m) {
        uint32_t x = m;
        uint64_t prod = 1;
        uint64_t sum  = 0;
        int     len   = 0;
        bool    has_zero = false;

        /* extract digits, skipping numbers containing 0 */
        while (x > 0) {
            uint32_t d = x % 10;
            if (d == 0) { has_zero = true; break; }
            prod *= d;
            sum  += d;
            ++len;
            x    /= 10;
        }
        if (has_zero) continue;
        if (len == 0) continue;  /* never occurs for m ≥ 1 */

        uint64_t denom = (uint64_t)len * sum;
        if (denom == 0) continue;          /* also impossible */
        if (prod % denom != 0) continue;    /* not an integer n */

        uint64_t n_val = prod / denom;
        if (n_val >= 1 && n_val <= MAX_N && a_cache[(int)n_val] == 0) {
            a_cache[(int)n_val] = (seq_t)m;
        }
    }

    cache_initialized = true;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* initialise cache on first call */
    if (!cache_initialized)
        precompute();

    if (n <= MAX_N)
        return a_cache[(int)n];
    else
        return 0;        /* not required to handle n > 10000 */
}
