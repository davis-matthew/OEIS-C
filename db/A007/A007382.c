/*
 * A007382  Number of strict (-1)st-order maximal independent sets in path
 *          graph.
 * https://oeis.org/A007382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor((n-1)/2)} C(n-i+1, i)
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"


seq_t a(idx_t n) {
    seq_t result = 0;
    idx_t max_i = (n - 1) / 2;
    for (idx_t i = 1; i <= max_i; i++) {
        result += binom(n - i + 1, i);
    }
    return result;
}
