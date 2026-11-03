/*
 * A077811  Euler's totient of numbers containing in their decimal
 *          representation only the digits 0 and 1.
 * https://oeis.org/A077811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = φ( A007088(n) ) (Euler's totient of the n‑th decimal
 *            number consisting solely of the digits 0 and 1)
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Convert n to the decimal number obtained by writing n in binary
   and interpreting each bit as a decimal digit.  For example
   n = 5 (binary 101) → 101. */
static seq_t binary_to_decimal(idx_t n) {
    seq_t value = 0;
    seq_t power = 1;          /* 10^k for the current digit */
    idx_t temp = n;           /* use a local copy to avoid modifying n */
    while (temp) {
        if (temp & 1)         /* bit is 1 → digit 1 */
            value += power;
        power *= 10;          /* next decimal position */
        temp >>= 1;           /* move to next bit */
    }
    return value;
}

/* Euler's totient function for an arbitrary-precision integer. */
static seq_t totient(seq_t n) {
    seq_t result = n;
    if (n == 0) return 0; /* not expected for the sequence */
    /* factor out powers of 2 */
    if ((n & 1) == 0) { /* n is even */
        while ((n & 1) == 0) n >>= 1;
        result -= result / 2;
    }
    /* trial division by odd primes */
    seq_t p = 3;
    /* loop as long as p*p <= n (remaining factor) */
    while (p * p <= n) {
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            result -= result / p;
        }
        p += 2;
    }
    /* if n has a remaining prime factor > 1 */
    if (n > 1) {
        result -= result / n;
    }
    return result;
}

seq_t a(idx_t n) {
    /* a(1) = φ(1) = 1, a(2) = φ(10) = 4, etc. */
    seq_t x = binary_to_decimal(n);
    return totient(x);
}
