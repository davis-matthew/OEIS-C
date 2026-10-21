/*
 * A062367  Multiplicative with a(p^e) = (e+1)*(e+2)*(2*e+3)/6.
 * https://oeis.org/A062367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} (e+1)*(e+2)*(2e+3)/6 (with a(1)=1)
 * Range:     u8 n <= 47, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;  /* undefined for n < 1 */

    uint64_t pr[32], exp[32];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, exp, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exp[i];
        seq_t term = (seq_t)(e + 1) * (seq_t)(e + 2) * (seq_t)(2 * e + 3) / (seq_t)6;
        result *= term;
    }
    return result;
}
