/*
 * A213707  Positions of zeros in A218254.
 * https://oeis.org/A213707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + A213706(n) = n + Sum_{r=0..n} s(r), where s(r) is the
 *            number of steps from r to 0 in A218254's row (s(0)=0,
 *            s(r)=1+s(r-A000120(r))); computed directly
 * Range:     u8 n <= 30, u16 n <= 714, u32 n <= 720, u64 n <= 720,
 *            u128 n <= 720, big any n
 * Validated: standard, pass, 77 terms through n = 684 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned A213707_pop(idx_t x)
{
    unsigned c = 0;
    while (x) { c += (unsigned)(x & 1); x >>= 1; }
    return c;
}

seq_t a(idx_t n)
{
    uint32_t *s = (uint32_t *)malloc(((size_t)n + 1) * sizeof(uint32_t));
    if (!s) abort();
    seq_t total = (seq_t)n;
    s[0] = 0;
    for (idx_t r = 1; r <= n; ++r) {
        s[r] = 1 + s[r - A213707_pop(r)];
        total += (seq_t)s[r];
    }
    free(s);
    return total;
}
