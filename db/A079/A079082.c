/*
 * A079082  Denominator of
 *          (prime(n)+1)*(prime(n+1)+1)/(4*(prime(n)*prime(n+1)+1)).
 * https://oeis.org/A079082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 30, u32 n <= 4611, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A079082
   Formula: a(n) = denominator of (prime(n)+1)*(prime(n+1)+1) /
                  (4*(prime(n)*prime(n+1)+1)) in lowest terms
   (offset 1: a(1) is the first listed term). */
#include "../utils/get_prime.h"

/* Euclidean algorithm for seq_t.  Works for any integer type used for seq_t
   because all operations are defined on seq_t (addition, subtraction,
   multiplication, division, modulus). */
static seq_t
seq_gcd(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n = 0 return 0 as a sentinel. */
    if (n == 0) {
        return 0;
    }

    /* Fetch the n‑th and (n+1)-th prime numbers. */
    seq_t p  = get_prime(n);
    seq_t p1 = get_prime(n + 1);

    /* Compute the numerator (p+1)*(p1+1). */
    seq_t num = (p + (seq_t)1) * (p1 + (seq_t)1);

    /* Compute the denominator 4*(p*p1+1). */
    seq_t denom = (seq_t)4 * (p * p1 + (seq_t)1);

    /* Reduce the fraction and return the simplified denominator. */
    seq_t g = seq_gcd(num, denom);
    return denom / g;
}
