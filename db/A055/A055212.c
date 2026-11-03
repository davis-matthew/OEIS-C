/*
 * A055212  Number of composite divisors of n.
 * https://oeis.org/A055212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = tau(n) - omega(n) - 1 (number of composite divisors of n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9790 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Count τ(n) (total divisor count) and ω(n) (distinct prime factors) */
static void count_tau_omega(idx_t n, seq_t *tau, seq_t *omega)
{
    seq_t tau_val = 1;   /* τ(1) = 1 */
    seq_t omega_val = 0; /* ω(1) = 0 */

    if (n == 1) {
        *tau = tau_val;
        *omega = omega_val;
        return;
    }

    idx_t tmp = n;
    idx_t p = 2;
    while (p * p <= tmp) {
        if (tmp % p == 0) {
            omega_val++;          /* new distinct prime factor */
            seq_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                exp++;
            }
            tau_val *= (exp + 1);
        }
        p++;
    }

    if (tmp > 1) { /* remaining prime factor */
        omega_val++;
        tau_val *= 2; // (1+1) for exponent 1
    }

    *tau = tau_val;
    *omega = omega_val;
}

seq_t a(idx_t n)
{
    seq_t tau, omega;
    count_tau_omega(n, &tau, &omega);
    return tau - omega - (seq_t)1;
}
