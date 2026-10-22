/*
 * A002109  Hyperfactorials: Product_{k = 1..n} k^k.
 * https://oeis.org/A002109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Product_{k=1..n} k^k, a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   a002109_pw
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a002109_pw.h"

static int a002109_have = 0;
static idx_t a002109_last_n = 0;
static seq_t a002109_last_v;

seq_t a(idx_t n) {
    seq_t result;
    idx_t k;
    if (a002109_have && a002109_last_n <= n) {
        result = a002109_last_v;
        k = a002109_last_n + 1;
    } else {
        result = 1;
        k = 1;
    }
    for (; k <= n; k++) {
        result *= a002109_pw((seq_t)k, k);
    }
    a002109_last_v = result;
    a002109_last_n = n;
    a002109_have = 1;
    return result;
}
