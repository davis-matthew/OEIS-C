/*
 * A171219  A138101(n)+A168142(n).
 * https://oeis.org/A171219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 1 + (2/3)*k*(k+1)*(2k+1) * where k is the smallest
 *            integer with * n ≤ k(k+1)(2k+1)/3. The sequence consists of *
 *            blocks of constant values A168388(2k+1) repeated * 2k² times.
 *            This short formula reproduces the same values
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   val_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/val_k.h"

/* cumulative length of the first k blocks */
static seq_t S(uint64_t k)
{
    seq_t kk = (seq_t)k;
    seq_t s  = kk * (kk + 1) * (2 * kk + 1);
    s /= 3;
    return s;
}

seq_t a(idx_t n)
{
    /* Find the smallest k with S(k) >= n */
    uint64_t k = 1;
    seq_t ns = (seq_t)n;
    while (S(k) < ns)
        ++k;
    return val_k(k);
}
