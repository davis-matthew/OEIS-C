/*
 * A074837  Numbers k such that the penultimate 3 divisors of k sum to k.
 * https://oeis.org/A074837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 60*floor((n-1)/4) + offsets[(n-1)%4] where offsets =
 *            {6,18,42,54}
 * Range:     u8 n <= 17, u16 n <= 4369, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t _offsets[4] = {6, 18, 42, 54};

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. */
    idx_t idx = n - 1;
    idx_t q   = idx / 4;   /* number of complete 60‑blocks before the term */
    idx_t r   = idx % 4;   /* position inside the current block */

    seq_t val = (seq_t)q * (seq_t)60 + _offsets[r];
    return val;
}
