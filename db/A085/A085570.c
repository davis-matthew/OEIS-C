/*
 * A085570  If n mod 2 = 0 then
 *          2*Sum(floor(C(n,w)/(2*w+1)),w=0..n/2-1)+floor(C(n,n/2)/(n+1))
 *          otherwise 2*Sum(floor(C(n,w)/(2*w+1)),w=0..(n-1)/2).
 * https://oeis.org/A085570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = if n mod 2 = 0 then 2*Sum_{w=0}^{n/2-1}
 *            floor(C(n,w)/(2*w+1)) + floor(C(n,n/2)/(n+1)), else
 *            2*Sum_{w=0}^{(n-1)/2} floor(C(n,w)/(2*w+1)). where C(n,w) =
 *            binomial coefficient. This implementation uses the provided
 *            binom function for safe arbitrary‑precision calculations
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 37 terms through n = 36 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    idx_t half = n / 2;
    seq_t sum = 0;

    if ((n & 1) == 0) {   /* even n */
        for (idx_t w = 0; w < half; ++w) {
            seq_t c   = binomial(n, w);
            seq_t div = (seq_t)(2 * w + 1);
            sum += c / div;
        }
        seq_t middle = binomial(n, half) / (seq_t)(n + 1);
        sum = sum * 2 + middle;
    } else {              /* odd n */
        for (idx_t w = 0; w <= half; ++w) {
            seq_t c   = binomial(n, w);
            seq_t div = (seq_t)(2 * w + 1);
            sum += c / div;
        }
        sum = sum * 2;
    }

    return sum;
}
