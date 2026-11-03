/*
 * A073032  A 4-cycle of the k => sigma(k)-k process, where sigma(k) is the
 *          sum of divisors of k (A000203).
 * https://oeis.org/A073032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 25 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A073032:
   a(1) = 18048976
   a(n+1) = σ(a(n)) – a(n)   (period‑4 cycle)
   The first four values are
   18048976, 20100368, 18914992, 19252208. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    static seq_t cycle[4];
    static bool init = false;

    if (!init) {
        cycle[0] = (seq_t)18048976ULL;
        for (int i = 0; i < 3; ++i) {
            seq_t s = sigma_of((idx_t)cycle[i]);   /* σ(k) */
            cycle[i + 1] = s - cycle[i];           /* next term */
        }
        init = true;
    }

    /* a(1) corresponds to cycle[0] */
    return cycle[(size_t)((n - 1ULL) & 3ULL)];
}
