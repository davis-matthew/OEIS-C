/*
 * A066990  In canonical prime factorization of n replace even exponents with
 *          2 and odd exponents with 1.
 * https://oeis.org/A066990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, mult, easy
 * Formula:   For n ≥ 1, let n = ∏ pᵉ be its prime factorization. a(n) = ∏
 *            p^k, where k = 1 if e is odd, and k = 2 if e is even.
 *            Equivalently, a(pᵉ) = p^(2–(e mod 2))
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses trial‑division factorisation.  It is O(√n) but fast
   enough for n up to at least a few million. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;          /* a(1) = 1 */

    seq_t result = (seq_t)1;
    idx_t temp = n;
    idx_t exp = 0;

    /* handle factor 2 */
    while ((temp & 1) == 0) {
        exp++;
        temp >>= 1;
    }
    if (exp > 0) {
        if (exp & 1)               /* odd exponent */
            result *= (seq_t)2;
        else                        /* even exponent */
            result *= (seq_t)4;     /* 2² */
    }

    /* handle odd primes */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        exp = 0;
        while (temp % p == 0) {
            exp++;
            temp /= p;
        }
        if (exp > 0) {
            if (exp & 1) 
                result *= (seq_t)p;            /* exponent 1 */
            else
                result *= (seq_t)p * (seq_t)p; /* exponent 2 */
        }
    }

    /* if something is left, it's a prime with exponent 1 */
    if (temp > 1)
        result *= (seq_t)temp;

    return result;
}
