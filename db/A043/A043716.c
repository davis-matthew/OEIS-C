/*
 * A043716  Numbers whose base-13 representation has an odd number of runs.
 * https://oeis.org/A043716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base-13 representation has an odd number of runs. */

#include <stdbool.h>

/* Return true if the base‑13 representation of x has an odd number of runs. */
static inline bool odd_runs(seq_t x)
{
    if (x == 0)
        return true;                 /* not used for n ≥ 1, but harmless */

    seq_t prev = x % (seq_t)13;
    x /= (seq_t)13;
    bool parity = true;              /* one run seen so far → odd */

    while (x > 0) {
        seq_t d = x % (seq_t)13;
        if (d != prev)
            parity = !parity;        /* toggle parity on change of digit */
        prev = d;
        x /= (seq_t)13;
    }
    return parity;
}

/* Return the n-th number (n ≥ 1) whose base‑13 representation has an odd number of runs. */
seq_t a(idx_t n)
{
    seq_t num = 1;
    idx_t  cnt = 0;

    for (;;) {
        if (odd_runs(num)) {
            ++cnt;
            if (cnt == n)
                return num;
        }
        ++num;
    }
}
