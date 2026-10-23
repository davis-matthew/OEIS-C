/*
 * A140728  Expansion of (phi(-q^3) * phi(-q^5) - phi(-q) * phi(-q^15)) / 2 in
 *          powers of q where phi() is a Ramanujan theta function.
 * https://oeis.org/A140728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   not stated
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A140728.
 *
 * The sequence is multiplicative:
 *   a(1) = 1
 *   For n > 1, let n = ∏ p_i ^ e_i.
 *   Then a(n) = ∏ a(p_i ^ e_i) where
 *
 *   a(2^e)   = (-1)^e * (1 - e)   (e > 0)
 *   a(3^e)   = (-1)^e
 *   a(5^e)   = (-1)^e
 *   If p ≡ 1 or 4 (mod 15) : a(p^e) = e + 1
 *   If p ≡ 2 or 8 (mod 15) : a(p^e) = (-1)^e * (e + 1)
 *   If p ≡ 7, 11, 13, or 14 (mod 15) :
 *       a(p^e) = (1 + (-1)^e) / 2    // 1 when e even, 0 when e odd
 *
 * The implementation factorises n by trial division (sufficient for the
 * required n ≤ ∼10⁴).  All arithmetic is performed in seq_t, which may be
 * a signed integer or an arbitrary‑precision integer type.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return value of a(p^e) according to the rules above. */
static seq_t prime_power_value(idx_t prime, idx_t exp)
{
    if (prime == 2) {
        /* exp > 0, handled by caller */
        seq_t sign = (exp & 1) ? (seq_t)-1 : (seq_t)1;
        return sign * ( (seq_t)(1) - (seq_t)exp );
    }
    if (prime == 3 || prime == 5) {
        seq_t sign = (exp & 1) ? (seq_t)-1 : (seq_t)1;
        return sign;
    }

    int r = (int)(prime % 15);
    seq_t sign = (exp & 1) ? (seq_t)-1 : (seq_t)1;

    if (r == 1 || r == 4) {
        return (seq_t)(exp + 1);
    }
    if (r == 2 || r == 8) {
        return sign * (seq_t)(exp + 1);
    }
    /* r == 7, 11, 13, or 14 */
    return ( (seq_t)1 + sign ) / (seq_t)2;   /* 1 if exp even, 0 if odd */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t remaining = n;

    /* factor 2 */
    idx_t exp = 0;
    while ((remaining & 1) == 0) {
        remaining >>= 1;
        ++exp;
    }
    if (exp > 0) {
        result *= prime_power_value(2, exp);
    }

    /* odd factors */
    for (idx_t p = 3; p <= remaining / p; p += 2) {
        if (remaining % p == 0) {
            idx_t e = 0;
            while (remaining % p == 0) {
                remaining /= p;
                ++e;
            }
            result *= prime_power_value(p, e);
        }
    }

    /* any remaining prime factor > 1 */
    if (remaining > 1) {
        result *= prime_power_value(remaining, 1);
    }

    return result;
}
