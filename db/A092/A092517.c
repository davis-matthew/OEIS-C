/*
 * A092517  Product of tau values for consecutive integers.
 * https://oeis.org/A092517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = tau(n) * tau(n + 1) (tau = number of divisors)
 * Range:     u8 n <= 2014, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Count divisors of a 64‑bit unsigned integer */
static seq_t count_divisors_u64(uint64_t x)
{
    if (x == 0) return 0;
    seq_t res = 1;
    for (uint64_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            int exp = 0;
            while (x % p == 0) {
                x /= p;
                ++exp;
            }
            res *= (exp + 1);
        }
    }
    if (x > 1) res *= 2;
    return res;
}

/* Compute a(n) = tau(n) * tau(n+1) */
seq_t a(idx_t n)
{
    /* n starts at 1 (offset 1) */
    seq_t tau_n     = count_divisors_u64((uint64_t)n);
    seq_t tau_next  = count_divisors_u64((uint64_t)n + 1);
    return tau_n * tau_next;
}
