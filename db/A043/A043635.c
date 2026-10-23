/*
 * A043635  Numbers whose base-9 representation has exactly 6 runs.
 * https://oeis.org/A043635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Numbers whose base‑9 representation has exactly 6 runs
 * Range:     u8 none, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int count_runs(seq_t v)
{
    const seq_t nine = (seq_t)9;
    int runs = 0;
    int prev = -1;
    while (v > 0) {
        int d = (int)(v % nine);
        if (d != prev) {
            ++runs;
            prev = d;
        }
        v /= nine;
    }
    return runs;
}

seq_t a(idx_t n)
{
    seq_t x = (seq_t)59787;     /* first term of the sequence */
    idx_t seen = (idx_t)0;
    while (1) {
        if (count_runs(x) == 6) {
            ++seen;
            if (seen == n)
                return x;
        }
        ++x;
    }
}
