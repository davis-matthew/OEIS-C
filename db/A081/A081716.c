/*
 * A081716  Smallest number x such that phi(x)=A000010(x) has exactly n
 *          different decimal digits.
 * https://oeis.org/A081716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A081716: Smallest prime p such that the number of distinct
   decimal digits in φ(p) = p - 1 equals n.  The function generates all
   n‑digit numbers with distinct digits in ascending numeric order,
   testing p = m + 1 for primality until the first prime is found. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"

/* recursive helper that generates numbers of length n with all digits
   distinct, in ascending order, and tests p = m + 1 for primality. */
static bool dfs_n(idx_t n, int pos, uint64_t curr, uint16_t used, seq_t *res)
{
    if (pos == n) {
        idx_t cand = (idx_t)(curr + 1);   /* p = m + 1 */
        if (is_prime(cand)) {
            *res = (seq_t)cand;
            return true;
        }
        return false;
    }
    for (int d = 0; d <= 9; ++d) {
        if (used & (1 << d)) continue;
        if (pos == 0 && d == 0) continue; /* leading zero not allowed */
        if (dfs_n(n, pos + 1, curr * 10 + d, used | (1 << d), res))
            return true;
    }
    return false;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    static seq_t cache[11] = {0};
    static bool ready[11]   = {0};

    if (n == 0) return 0;           /* undefined for n < 1 */
    if (n > 10) return 0;           /* n > 10 impossible */

    if (!ready[n]) {
        seq_t result = 0;
        if (dfs_n((idx_t)n, 0, 0, 0, &result))
            cache[n] = result;
        ready[n] = true;
    }
    return cache[n];
}
