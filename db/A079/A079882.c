/*
 * A079882  A run of 2^n 1's followed by a run of 2^n 2's, for n=0, 1, 2, ...
 * https://oeis.org/A079882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if the position falls in the 1‑run of its block,
 *            otherwise 2. The block containing index n has length 2^(k+1)
 *            where k = floor(log2(n+2)) – 1, i.e. its start is 2^(k+1) – 2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   A079882
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Author: ChatGPT – simple logarithmic computation */

#include <stdint.h>
#include "../utils/A079882.h"

seq_t a(idx_t n) { return A079882(n); }
