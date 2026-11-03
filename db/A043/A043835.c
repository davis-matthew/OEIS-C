/*
 * A043835  Numbers k such that number of runs in the base 4 representation of
 *          k is congruent to 3 mod 5.
 * https://oeis.org/A043835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth integer k such that the number of runs in the base-4
 *            representation of k is congruent to 3 mod 5
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static inline bool runs_mod5_eq3(seq_t k)
{
    int runs = 0;
    int prev = -1;
    seq_t t = k;
    while (t > 0)
    {
        int d = (int)(t % 4);
        if (d != prev)
        {
            runs++;
            prev = d;
        }
        t /= 4;
    }
    return (runs % 5) == 3;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset starts at 1 */
    idx_t found = 0;
    seq_t k = 1;
    while (1)
    {
        if (runs_mod5_eq3(k))
        {
            found++;
            if (found == n)
                return k;
        }
        k++;
    }
}
