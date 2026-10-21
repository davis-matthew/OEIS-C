/*
 * A071363  Largest n-digit prime with strictly increasing digits.
 * https://oeis.org/A071363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   a(n) is the largest n‑digit prime whose decimal digits increase
 *            strictly
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_uint64.h"

static bool dfs(int idx, int last, int n, uint64_t val, seq_t *best)
{
    if (idx == n) {
        if (is_prime_uint64(val)) {
            *best = (seq_t)val;
            return true;
        }
        return false;
    }
    // choose next digit in descending order to generate the largest numbers first
    for (int d = 9; d > last; --d) {
        uint64_t nval = val * 10 + d;
        if (dfs(idx + 1, d, n, nval, best))
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, but just in case */
    if (n > 9) return 0;           /* no 9‑digit increasing‑digit number exists */
    seq_t best = 0;
    dfs(0, 0, (int)n, 0, &best);
    return best;
}
