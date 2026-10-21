/*
 * A071361  Largest n-digit prime with strictly decreasing digits.
 * https://oeis.org/A071361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 4, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the largest n-digit prime with strictly decreasing digits. */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Depth‑first search for the largest decreasing‑digit prime. */
static bool search(int max_digit, int needed, seq_t cur, seq_t *res)
{
    if (needed == 0) {
        if (is_prime_big(cur)) {
            *res = cur;
            return true;
        }
        return false;
    }
    for (int d = max_digit; d >= needed - 1; --d) {
        seq_t next = (cur * 10) + (seq_t)d;
        if (search(d - 1, needed - 1, next, res))
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0 || n > 10)
        return 0;
    seq_t result = 0;
    search(9, (int)n, 0, &result);   /* result holds the first prime found   */
    return result;
}
