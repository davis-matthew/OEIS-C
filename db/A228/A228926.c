/*
 * A228926  Sum(m^(n+1), m=1...n-1) modulo n.
 * https://oeis.org/A228926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum(m^(n+1), m=1...n-1) modulo n
 * Range:     u8 n <= 297, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 93 terms through n = 8639 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/powmod.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t mod = (seq_t)n;
    seq_t sum = 0;
    for (idx_t m = 1; m < n; ++m) {
        seq_t term = powmod((seq_t)m, n + 1, mod);
        sum += term;
        if (sum >= mod)
            sum -= mod;          /* keep sum < mod */
    }
    return sum;
}
