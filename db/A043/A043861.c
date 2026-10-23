/*
 * A043861  Numbers n such that number of runs in the base 4 representation of
 *          n is congruent to 3 mod 9.
 * https://oeis.org/A043861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A043861 – numbers n with (runs in base‑4 of n) ≡ 3 (mod 9).
    The function scans successive integers, counts runs in base 4 and
    accumulates those with runs%9==3 until the desired index is reached. */
#include <stdint.h>
#include <stdbool.h>

/* Count the number of runs in the base‑4 representation of x.
   A run is a consecutive run of the same digit. */
static inline int count_runs(idx_t x)
{
    if (x == 0)
        return 1;               /* base‑4 of 0 is “0”, one run */
    int runs = 1;
    int prev = x % 4;
    x /= 4;
    while (x) {
        int d = x % 4;
        if (d != prev) {
            ++runs;
            prev = d;
        }
        x /= 4;
    }
    return runs;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* undefined for 0, but returns 0 */

    idx_t count = 0;
    idx_t value = 1;

    for (;; ++value) {
        if (count_runs(value) % 9 == 3) {
            ++count;
            if (count == n)
                return (seq_t)value;
        }
    }
}
