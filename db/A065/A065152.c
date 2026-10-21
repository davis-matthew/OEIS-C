/*
 * A065152  a(n) = cototient(totient(n)) - totient(cototient(n)).
 * https://oeis.org/A065152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = cototient(totient(n)) - totient(cototient(n))
 * Range:     i8 n <= 180, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   factorize
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factorize.h"

/* Compute Euler's totient function for a 64‑bit integer.
   Returns 0 for n = 0 (used for cototient(1)). */
static unsigned long long phi_u64(unsigned long long n)
{
    if (n == 0) return 0;
    unsigned long long phi = n;
    uint64_t primes[64];
    int exps[64];
    size_t cnt = factorize(n, primes, exps);
    for (size_t i = 0; i < cnt; ++i) {
        phi -= phi / primes[i];
    }
    return phi;
}

/* OEIS sequence A065152 */
seq_t a(idx_t n)
{
    unsigned long long n_ul = (unsigned long long)n;
    unsigned long long phi_n = phi_u64(n_ul);          /* φ(n) */
    unsigned long long cot_n = n_ul - phi_n;           /* n – φ(n) */

    unsigned long long phi_phi_n = phi_u64(phi_n);     /* φ(φ(n)) */
    unsigned long long cot_phi_n = phi_n - phi_phi_n; /* cototient(φ(n)) */

    unsigned long long phi_cot_n = phi_u64(cot_n);     /* φ(cototient(n)) */

    seq_t result = (seq_t)cot_phi_n - (seq_t)phi_cot_n;
    return result;
}
