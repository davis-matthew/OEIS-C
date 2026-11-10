/*
 * A096119  A050029(2^n + 1).
 * https://oeis.org/A096119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A050029(2^n + 1), where A050029(1)=A050029(2)=1,
 *            A050029(3)=2, A050029(i) = A050029(i-1) + A050029(2^(p+1)+2-i)
 *            for i>=4, 2^p < i-1 <= 2^(p+1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 14 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t A096119_a050029(idx_t N)
{
    if (N <= 2) return (seq_t)1;
    if (N == 3) return (seq_t)2;
    seq_t *c = mem_new(seq_t, (size_t)N + 1);
    c[1] = (seq_t)1;
    c[2] = (seq_t)1;
    c[3] = (seq_t)2;
    idx_t power = 4; /* smallest power of 2 >= i-1, starting for i=4 (i-1=3) */
    for (idx_t i = 4; i <= N; ++i) {
        while (power < i - 1) power <<= 1;
        idx_t m = power + 2 - i;
        c[i] = c[i - 1] + c[m];
    }
    seq_t r = c[N];
    mem_free(c);
    return r;
}

seq_t a(idx_t n)
{
    return A096119_a050029(((idx_t)1 << n) + 1);
}
