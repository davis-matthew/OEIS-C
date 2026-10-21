/*
 * A239614  a(n) = A239611(n) / A079458(n).
 * https://oeis.org/A239614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(2*n) = 2 * tau(n), a(2*n+1) = tau(2*n+1)
 * Range:     u8 n <= 2101, u16 n <= 2101, u32 n <= 2101, u64 n <= 2101,
 *            u128 n <= 2101, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    if ((n & 1ULL) == 0) {           /* n is even: n = 2*m */
        idx_t m = n >> 1;            /* m = n/2 */
        size_t tau_m = divisor_count_u64(m);
        return (seq_t)(2 * tau_m);
    } else {                         /* n is odd */
        size_t tau_n = divisor_count_u64(n);
        return (seq_t)tau_n;
    }
}
