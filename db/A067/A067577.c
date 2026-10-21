/*
 * A067577  a(n) = Product_{i=1..n} sigma(i) * Sum_{i=1..n} 1/sigma(i).
 * https://oeis.org/A067577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (∏_{i=1..n} σ(i)) * (∑_{i=1..n} 1/σ(i))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n<1, harmless */
    seq_t product = 1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t s = sigma_of(i);
        product *= s;
    }
    seq_t result = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t s = sigma_of(i);
        result += product / s;
    }
    return result;
}
