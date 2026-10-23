/*
 * A181833  The number of positive integers <= n that are not strongly prime
 *          to n.
 * https://oeis.org/A181833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - φ(n) + τ(n-1) for n>0 (a(0)=a(1)=0)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* φ(n) – Euler's totient, τ(m) – number of divisors of m   */

#include <stdint.h>

static seq_t phi_u64(uint64_t x)
{
    if (x == 0) return 0;
    seq_t result = (seq_t)x;
    uint64_t temp = x;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            while (temp % p == 0) temp /= p;
            result -= result / p;
        }
    }
    if (temp > 1) result -= result / temp;
    return result;
}

static seq_t tau_u64(uint64_t x)
{
    if (x == 0) return 1;    /* unused for n>1 but harmless */
    seq_t count = 1;
    uint64_t temp = x;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            count *= (exp + 1);
        }
    }
    if (temp > 1) count *= 2;
    return count;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    seq_t phi_n   = phi_u64((uint64_t)n);
    seq_t tau_nm1 = tau_u64((uint64_t)(n - 1));
    return (seq_t)n - phi_n + tau_nm1;
}
