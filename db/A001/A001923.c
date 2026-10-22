/*
 * A001923  a(n) = Sum_{k=1..n} k^k.
 * https://oeis.org/A001923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{k=1..n} k^k, i.e. a(n) = a(n-1) + n^n, a(0) = 0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   a1923_kpow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a1923_kpow.h"

static seq_t a1923_sum = 0;
static idx_t a1923_upto = 0;

seq_t a(idx_t n) {
    idx_t k;
    if (n < a1923_upto) {
        a1923_sum = 0;
        a1923_upto = 0;
    }
    for (k = a1923_upto + 1; k <= n; k++) {
        a1923_sum += a1923_kpow((seq_t)k, k);
    }
    a1923_upto = n;
    return a1923_sum;
}
