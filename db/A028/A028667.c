/*
 * A028667  Galois numbers for p=5; order of group AGL(n,5).
 * https://oeis.org/A028667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5^n * Π_{k=0..n-1} (5^n - 5^k) (= 5^{n(n+1)/2} *
 *            Π_{j=1..n} (5^j - 1))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"   /* provides seq_t pow_uint(uint64_t base, idx_t exp) */

seq_t a(idx_t n) {
    /* exponent e = n*(n+1)/2 */
    idx_t e = n * (n + 1) / 2;

    /* base term: 5^e */
    seq_t result = pow_uint(5, e);

    /* compute product Π_{j=1..n} (5^j - 1) */
    seq_t pow5j = (seq_t)5;          /* current 5^j, starts with j=1 */
    for (idx_t j = 1; j <= n; ++j) {
        seq_t term = pow5j - (seq_t)1;   /* (5^j - 1) */
        result *= term;                  /* multiply into result */
        pow5j *= (seq_t)5;                /* advance to 5^{j+1} */
    }

    return result;
}
