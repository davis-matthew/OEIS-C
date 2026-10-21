/*
 * A080063  a(n) = n mod (lpf(n)+1), where lpf(n) is the least prime dividing
 *          n (A020639).
 * https://oeis.org/A080063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n mod (lpf(n)+1) where lpf(n) is the least prime divisor
 *            of n
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   lpf_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes on-demand; works for any idx_t and seq_t. */

#include <stdint.h>
#include "../utils/lpf_idx.h"

seq_t a(idx_t n)
{
    idx_t p = lpf_idx(n);
    return (seq_t)n % ((seq_t)p + 1);
}
