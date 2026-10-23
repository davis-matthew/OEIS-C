/*
 * A043608  Numbers whose base-5 representation has exactly 8 runs.
 * https://oeis.org/A043608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer whose base-5 representation has
 *            * exactly 8 runs (runs = maximal contiguous blocks of equal
 *            digits). * This is computed by enumerating base‑5 strings with 8
 *            runs in ascending * order and selecting the n‑th one
 * Range:     u8 none, u16 none, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static bool dfs(int pos, int total_len, seq_t val, uint8_t prev,
                idx_t target, idx_t *count, seq_t *result)
{
    if (pos == total_len) {
        ++(*count);
        if (*count == target) {
            *result = val;
            return true;
        }
        return false;
    }

    if (pos == 0) {
        for (uint8_t d = 1; d <= 4; ++d) {
            if (dfs(pos + 1, total_len, val * (seq_t)5 + (seq_t)d,
                    d, target, count, result))
                return true;
        }
    } else {
        for (uint8_t d = 0; d < 5; ++d) {
            if (d == prev) continue;
            if (dfs(pos + 1, total_len,
                    val * (seq_t)5 + (seq_t)d, d,
                    target, count, result))
                return true;
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t target = n;
    idx_t count  = 0;
    seq_t value = 0;

    for (int len = 8; ; ++len) {
        if (dfs(0, len, (seq_t)0, 0, target, &count, &value))
            return value;
    }
}
