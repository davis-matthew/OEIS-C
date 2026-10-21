/*
 * A071362  Smallest n-digit prime with strictly increasing digits.
 * https://oeis.org/A071362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   simple_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the smallest n‑digit prime with strictly increasing decimal digits. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/simple_prime.h"

/* Recursive search for the smallest increasing‑digit prime.
   pos  – current position (0‑based),
   start – smallest digit allowed at this position,
   cur  – number built so far,
   need – total number of digits,
   result – location where the found prime is stored. */
static bool dfs(idx_t pos, int start, uint64_t cur, idx_t need, seq_t *result)
{
    if (pos == need) {
        if (simple_prime(cur)) {
            *result = (seq_t)cur;
            return true;
        }
        return false;
    }
    /* Upper bound for the current digit so that enough digits remain. */
    int max_digit = 10 - (int)(need - pos);
    for (int d = start; d <= max_digit; ++d) {
        uint64_t next = cur * 10ULL + (uint64_t)d;
        if (dfs(pos + 1, d + 1, next, need, result))
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    const idx_t MAX_N = 9;            /* 9 different non‑zero digits possible */
    static seq_t cache[10];           /* 1‑based cache */
    static bool done[10] = { false };

    if (n < 1 || n > MAX_N) return 0; /* no such prime for n > 9 */

    if (done[n]) return cache[n];

    seq_t value = 0;
    dfs(0, 1, 0, n, &value);         /* search once */
    cache[n] = value;
    done[n] = true;
    return value;
}
