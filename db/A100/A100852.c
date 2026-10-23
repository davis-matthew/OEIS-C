/*
 * A100852  Triangle read by rows: T(n,k) = 2^k * 3^n, 0 <= k <= n.
 * https://oeis.org/A100852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 3^r * 2^k where r = floor((sqrt(8*n+1)-1)/2) is the row
 *            index and k = n - r*(r+1)/2 is the column index within that row
 * Range:     u8 n <= 11, u16 n <= 32, u32 n <= 102, u64 n <= 349,
 *            u128 n <= 1323, big any n
 * Helpers:   row_of_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/row_of_idx.h"

/* Compute 3^exp and 2^exp by exponentiation by squaring. */
static inline seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1ul) result *= base;
        base *= base;
        exp >>= 1ul;
    }
    return result;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    idx_t r = row_of_idx(n);
    idx_t tri = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t k = n - tri;
    seq_t pow3 = pow_seq((seq_t)3, r);
    seq_t pow2 = pow_seq((seq_t)2, k);
    return pow3 * pow2;
}
