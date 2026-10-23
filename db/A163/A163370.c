/*
 * A163370  a(n) = phi(sigma(phi(n))).
 * https://oeis.org/A163370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = φ(σ(φ(n)))
 * Range:     u8 n <= 184, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Euler's totient φ for a 64‑bit integer */
static seq_t phi_uint64(uint64_t m)
{
    if (m <= 1) return (seq_t)m;          /* φ(0)=0, φ(1)=1          */
    seq_t result = (seq_t)m;
    uint64_t temp  = m;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            /* remove all occurrences of prime p */
            while (temp % p == 0) temp /= p;
            result = result / (seq_t)p * ((seq_t)p - 1);
        }
    }
    if (temp > 1) {                       /* remaining prime factor    */
        result = result / (seq_t)temp * ((seq_t)temp - 1);
    }
    return result;
}

/* Sum of divisors σ for a 64‑bit integer */
static seq_t sigma_uint64(uint64_t m)
{
    if (m == 0) return (seq_t)0;
    seq_t result = (seq_t)1;
    uint64_t temp  = m;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t power = 1;
            seq_t term = 1;
            while (temp % p == 0) {
                temp /= p;
                power *= p;
                term += power;
            }
            result *= term;
        }
    }
    if (temp > 1) {                       /* remaining prime factor    */
        result *= ((seq_t)1 + (seq_t)temp);
    }
    return result;
}

/* OEIS sequence A163370 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* not used for the offset   */
    seq_t phi_n     = phi_uint64((uint64_t)n);
    seq_t sigma_phi = sigma_uint64((uint64_t)phi_n);
    seq_t ans       = phi_uint64((uint64_t)sigma_phi);
    return ans;
}
