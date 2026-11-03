/*
 * A128261  a(n) = tau(n) + (n-1)*tau(n-1).
 * https://oeis.org/A128261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = tau(n) + (n-1)*tau(n-1)
 * Range:     u8 n <= 36, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 74 terms through n = 996 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Count divisors of a 64-bit integer n (n > 0). */
static inline seq_t _tau_u64(uint64_t n)
{
    if (n == 0) return 0;
    seq_t result = (seq_t)1;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t cnt = 0;
            while (m % p == 0) {
                m /= p;
                ++cnt;
            }
            result *= (seq_t)(cnt + 1);
        }
    }
    if (m > 1) result *= (seq_t)2;  /* remaining prime factor */
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined offset, return 0 for safety */
    if (n == 1) return (seq_t)1;   /* tau(1)=1, (n-1)*tau(0)=0 */

    seq_t tau_n      = _tau_u64((uint64_t)n);
    seq_t tau_n_minus = _tau_u64((uint64_t)(n - 1));
    seq_t term = tau_n + (seq_t)(n - 1) * tau_n_minus;
    return term;
}
