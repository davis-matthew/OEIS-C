/*
 * A172286  Numbers of circuits of length 2n in K_{n,n} (the complete
 *          bipartite graph on 2n vertices).
 * https://oeis.org/A172286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2 * n^(2*n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Validated: standard, pass, 29 terms through n = 73 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n) {
    /* compute 2 * n^(2*n) for n >= 1 */
    seq_t base = (seq_t)n;
    seq_t exp  = (seq_t)(2 * n);
    seq_t res  = ipow(base, exp);
    res *= 2;
    return res;
}
