/*
 * A222638  Product, for k <= n, of the squarefree parts of the total number
 *          of arrangements of a set with k elements.
 * https://oeis.org/A222638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} A007913(A000522(k)), A000522(k) =
 *            k*A000522(k-1)+1, A000522(0)=1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 13 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t A007913_a(seq_t n) {
    seq_t result = 1;
    seq_t temp = n;
    seq_t i = 2;
    while (i <= temp / i) {
        seq_t i_sq = i * i;
        while (temp % i_sq == 0) {
            temp = temp / i_sq;
        }
        if (temp % i == 0) {
            result = result * i;
            temp = temp / i;
        }
        i++;
    }
    if (temp > 1) {
        result = result * temp;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t prod = 1;
    seq_t e = 1; /* A000522(0) */
    for (idx_t k = 0; k <= n; k++) {
        if (k > 0) {
            e = (seq_t)k * e + 1;
        }
        prod = prod * A007913_a(e);
    }
    return prod;
}
