/*
 * A098350  Multiplication table of the primes read by antidiagonals.
 * https://oeis.org/A098350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = prime(i) * prime(j), where (i,j) are the indices * of the
 *            prime multiplication table read by antidiagonals. * The pair
 *            (i,j) is determined from n by: * Find the smallest integer s
 *            with (s‑1)*s/2 ≥ n. * Let prev = (s‑2)*(s‑1)/2. * Set offset = n
 *            – prev. Then i = offset, j = s – offset
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;
    idx_t s = 2;
    idx_t cum = 1;                     // (s-1)*s/2 for s=2
    while (cum < n) {
        ++s;
        cum += s - 1;                  // add size of current diagonal
    }
    idx_t prevcum = cum - (s - 1);     // sum up to previous diagonal
    idx_t offset = n - prevcum;        // 1 .. s-1
    idx_t i = offset;                  // row index
    idx_t j = s - offset;              // column index

    uint32_t pi = get_prime(i);
    uint32_t pj = get_prime(j);
    return (seq_t)pi * pj;
}
