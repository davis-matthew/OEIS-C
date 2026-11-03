/*
 * A157021  a(n)=sum{k=0..floor(n/2), C(n,2k)*A000108(floor(k/2))}. Inverse
 *          binomial transform is aeration of doubled Catalan numbers.
 * https://oeis.org/A157021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n)=sum_{k=0..floor(n/2)} C(n,2k)*Catalan(floor(k/2))
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 29, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 31 (2026-11-01)
 * Helpers:   binomial, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/catalan.h"

seq_t a(idx_t n) {
    seq_t result = 0;
    idx_t max_k = n / 2;
    for (idx_t k = 0; k <= max_k; ++k) {
        idx_t even = k << 1;          /* 2*k */
        seq_t bin = binomial(n, even);   /* C(n,2k) */
        seq_t cat = catalan(k / 2);   /* Catalan(floor(k/2)) */
        result += bin * cat;
    }
    return result;
}
