/*
 * A205381  s(A205379), where s(j)=(2j-1)^2.
 * https://oeis.org/A205381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*j-1)^2 where j = A205379(n) is the index of the
 *            earlier term in the first collision of (2i-1)^2 mod n, found by
 *            search
 * Range:     u8 n <= 18, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    idx_t *seen = (idx_t *)calloc((size_t)n, sizeof(idx_t));
    if (!seen) abort();
    idx_t res = 1;
    for (idx_t k = 1; ; ++k)
    {
        idx_t o = 2 * k - 1;
        idx_t m = o % n;
        idx_t r = (idx_t)(((unsigned __int128)m * m) % n);
        if (seen[r])
        {
            idx_t j = seen[r];
            res = j;
            break;
        }
        seen[r] = k;
    }
    free(seen);
    seq_t o = (seq_t)(2 * res - 1);
    return o * o;
}
