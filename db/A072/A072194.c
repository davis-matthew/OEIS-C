/*
 * A072194  Replace all prime factors p of n with n-p.
 * https://oeis.org/A072194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n)=1 if n==1; otherwise a(n)=∏_{p^e∥n}(n-p)^e; for prime n
 *            result 0
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 63, u64 n <= 383,
 *            u128 n <= 4095, big any n
 * Helpers:   factor_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_uint64.h"

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)n;          /* a(1)=1 (a(0) unused) */

    uint64_t x = (uint64_t)n;
    uint64_t fac[20];
    int exp[20];
    int cnt = 0;
    factor_uint64(x, fac, exp, &cnt);

    seq_t prod = 1;
    for (int i = 0; i < cnt; ++i) {
        seq_t term = (seq_t)(n - fac[i]);   /* (n - p) */
        for (int j = 0; j < exp[i]; ++j) {
            prod *= term;
        }
    }
    return prod;
}
