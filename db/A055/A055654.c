/*
 * A055654  Difference between n and the result of "Phi-summation" over
 *          unitary divisors of n.
 * https://oeis.org/A055654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - Sum_{u|n, gcd(u,n/u)=1} φ(u) (unitary φ‑summation)
 * Range:     u8 n <= 431, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return a(n) for the OEIS A055654 sequence.  */
/* The function is thread‑safe and may be called in any order.  */
seq_t a(idx_t n)
{
    if (n == 1)                /* empty product → 1,  a(1) = 1‑1 = 0 */
        return (seq_t)0;

    seq_t prod = 1;            /* product over prime powers 1 + φ(p⁽ᵉ⁾) */
    idx_t m = n;               /* working copy for trial division */

    /* handle factor 2 separately to avoid checking even numbers later */
    if ((m & 1ULL) == 0) {
        seq_t p_pow = 1;
        while ((m & 1ULL) == 0) {
            m >>= 1;
            p_pow *= (seq_t)2;
        }
        seq_t phi_p = p_pow - p_pow / (seq_t)2;  /* φ(2ᵉ) */
        prod *= 1 + phi_p;
    }

    /* odd prime factors */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p != 0)
            continue;
        seq_t p_pow = 1;
        while (m % p == 0) {
            m /= p;
            p_pow *= (seq_t)p;
        }
        seq_t phi_p = p_pow - p_pow / (seq_t)p;  /* φ(pᵉ) */
        prod *= 1 + phi_p;
    }

    /* if what remains is a prime > sqrt(original n) */
    if (m > 1) {
        seq_t p_pow = (seq_t)m;          /* exponent 1 */
        seq_t phi_p = p_pow - 1;        /* φ(p) = p‑1 */
        prod *= 1 + phi_p;
    }

    return (seq_t)n - prod;
}
