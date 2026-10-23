/*
 * A104973  Numbers with 6 distinct digits {1,2,3,4,5,6} such that all
 *          adjacent digits (as well as first and last digits) are coprime.
 * https://oeis.org/A104973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  full, nonn, base, fini
 * Formula:   a(n) is the nth smallest 6‑digit number containing each of 1..6
 *            once such that any two adjacent digits (including the last and
 *            first) are coprime
 * Range:     u8 none, u16 none, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   coprime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/coprime_small.h"

static void backtrack(int depth, int cur[6], bool used[7], seq_t *out, idx_t *cnt)
{
    if (depth == 6) {
        if (!coprime_small(cur[5], cur[0]))
            return;
        seq_t value = 0;
        for (int i = 0; i < 6; ++i)
            value = value * 10 + cur[i];
        out[(*cnt)++] = value;
        return;
    }
    for (int d = 1; d <= 6; ++d) {
        if (!used[d]) {
            if (depth > 0 && !coprime_small(cur[depth - 1], d))
                continue;
            used[d] = true;
            cur[depth] = d;
            backtrack(depth + 1, cur, used, out, cnt);
            used[d] = false;
        }
    }
}

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t values[720];
    static idx_t count = 0;
    if (!initialized) {
        bool used[7] = {false};
        int cur[6];
        backtrack(0, cur, used, values, &count);
        initialized = true;
    }
    if (n == 0 || n > count)
        return 0;
    return values[n - 1];
}
