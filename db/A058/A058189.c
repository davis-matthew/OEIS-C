/*
 * A058189  Number of increasing geometric progressions ending in n (in the
 *          positive integers), including those of length 1 or 2.
 * https://oeis.org/A058189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ∑_{p ≥ 2, p² | n} φ(p)·(v_p(n)-1) where v_p(n) is the
 *            exponent of p in the factorisation of n and φ is Euler's totient
 *            function
 * Range:     u8 n <= 242, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 87 terms through n = 9434 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t p = 2;
    while (p <= n / p) {                 /* p² ≤ n  */
        idx_t temp = n;
        int e = 0;
        while (temp % p == 0) {          /* count exponent e of p in n  */
            ++e;
            temp /= p;
        }
        if (e >= 2) {
            seq_t phi_p = totient(p);
            sum += phi_p * (seq_t)(e - 1);
        }
        ++p;
    }
    return (seq_t)n + sum;
}
