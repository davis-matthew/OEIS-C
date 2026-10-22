/*
 * A005014  Certain subgraphs of a directed graph (inverse binomial transform
 *          of A005321).
 * https://oeis.org/A005014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^n-2)*a(n-1) - (-1)^n, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   A001832_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/A001832_pow2.h"

static idx_t last_n = 0;
static seq_t last_val;

seq_t a(idx_t n) {
    seq_t v;
    idx_t i;
    if (n == 0) return 0;
    if (last_n >= 1 && last_n <= n) {
        i = last_n;
        v = last_val;
    } else {
        i = 1;
        v = 1;
    }
    while (i < n) {
        i++;
        seq_t factor = A001832_pow2(i) - 2;
        if (i & 1)
            v = factor * v + 1;
        else
            v = factor * v - 1;
    }
    last_n = n;
    last_val = v;
    return v;
}
