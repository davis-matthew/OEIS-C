/*
 * A167362  Totally multiplicative sequence with a(p) = (p-3)*(p+3) = p^2-9
 *          for prime p.
 * https://oeis.org/A167362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = ∏_{p^e∥n} (p^2‑9)^e, a(1) = 1
 * Range:     i8 n <= 12, i16 n <= 127, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the sequence A167362 for any n >= 1.  The implementation
   uses simple trial division and integer arithmetic only, so it
   works with any definition of seq_t and idx_t provided by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* Indexing starts at 1; 0 is unused */
    if (n == 1) return 1;      /* a(1) = 1 */

    idx_t temp   = n;          /* working copy of n */
    seq_t result = 1;

    /* Process factor 2 separately (fast path) */
    idx_t exp = 0;
    while ((temp & 1ULL) == 0) {
        temp >>= 1;
        exp++;
    }
    if (exp > 0) {
        seq_t base = (seq_t)2 * (seq_t)2 - (seq_t)9;   /* (2^2‑9) = -5 */
        seq_t pow  = 1;
        for (idx_t i = 0; i < exp; ++i) pow = pow * base;
        result = result * pow;
    }

    /* Handle odd prime factors */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            seq_t base = (seq_t)p * (seq_t)p - (seq_t)9;  /* p^2‑9 */
            seq_t pow  = 1;
            for (idx_t i = 0; i < exp; ++i) pow = pow * base;
            result = result * pow;
        }
    }

    /* If anything remains, it is a prime > sqrt(n) with exponent 1 */
    if (temp > 1) {
        seq_t base = (seq_t)temp * (seq_t)temp - (seq_t)9;
        result = result * base;
    }

    return result;
}
