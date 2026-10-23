/*
 * A138810  Expansion of (8 / 7) * (1 - eta(q)^7 / eta(q^7)) - 7 * (eta(q) *
 *          eta(q^7))^3 in powers of q.
 * https://oeis.org/A138810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 16, i32 n <= 3132, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A138810: multiplicative function defined by
 *   a(1) = 1
 *   a(p^e) = 1                         if p = 7
 *   a(p^e) = ((p^2)^(e+1) - 1)/(p^2-1) if p ≡ 1,2,4 (mod 7)
 *   a(p^e) = (1 - (-p^2)^(e+1))/(p^2+1) if p ≡ 3,5,6 (mod 7)
 *   a(mn) = a(m) * a(n) for (m,n)=1
 *
 * The function returns the value a(n) for any n ≥ 1.
 * It works with any seq_t integer type (int8_t … __int128 or
 * arbitrary‑precision).  The implementation uses only integer
 * arithmetic and is O(√n) per call.
 */

#include <stdbool.h>
#include <stdint.h>

/* local helper: power of a seq_t base to a small non‑negative integer exp */
static seq_t pow_seq_uint64(seq_t base, int exp)
{
    seq_t r = 1;
    for (int i = 0; i < exp; ++i) r *= base;
    return r;
}

/* value of a(p^e) for an arbitrary prime p and exponent e ≥ 1 */
static seq_t a_prime_pow(uint64_t p, int e)
{
    if (p == 7) return (seq_t)1;            /* a(7^e) = 1 */

    seq_t p2 = (seq_t)p * (seq_t)p;         /* p² */
    int m   = e + 1;                        /* exponent in the formulas */

    /* Determine residue class of p modulo 7 */
    uint64_t mod = p % 7U;
    if (mod == 1U || mod == 2U || mod == 4U) {
        /* a(p^e) = ((p^2)^(e+1) - 1)/(p^2 - 1) */
        seq_t num = pow_seq_uint64(p2, m) - 1;
        seq_t den = p2 - 1;
        return num / den;
    } else { /* mod == 3,5,6 */
        /* (-p^2)^(m) = (+p^(2m)) if m even, -p^(2m) if m odd */
        seq_t powp2 = pow_seq_uint64(p2, m);          /* p^(2m) */
        seq_t numerator = (m & 1) ? (1 + powp2) : (1 - powp2);
        seq_t den = p2 + 1;
        return numerator / den;
    }
}

/* Main API routine */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* out of range, harmless */

    uint64_t m = (uint64_t)n;
    seq_t result = 1;

    /* Handle factor 2 separately */
    if ((m & 1ULL) == 0) {
        int exp = 0;
        while ((m & 1ULL) == 0) { m >>= 1; ++exp; }
        result *= a_prime_pow(2, exp);
    }

    /* Handle odd prime factors */
    for (uint64_t d = 3; d * d <= m; d += 2) {
        if (m % d == 0) {
            int exp = 0;
            while (m % d == 0) { m /= d; ++exp; }
            result *= a_prime_pow(d, exp);
        }
    }

    /* If m > 1, it is a remaining prime factor */
    if (m > 1) {
        result *= a_prime_pow(m, 1);
    }

    return result;
}
