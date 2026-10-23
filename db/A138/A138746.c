/*
 * A138746  Expansion of 1 - eta(q) * eta(q^3) * eta(q^4)^3 / (eta(q^2)^2 *
 *          eta(q^12)) in powers of q.
 * https://oeis.org/A138746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   multiplicative sequence A138746 a(1) = 1 a(2^e) = -1 for e > 0
 *            a(3^e) = 2 - (-1)^e a(p^e) = e+1 if p ≡ 1,5 (mod 12) a(p^e) =
 *            (1+(-1)^e)/2 (=1 if e even, 0 if e odd) if p ≡ 7,11 (mod 12)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* helper: contribution of a single prime power p^e */
static seq_t prime_power_contrib(uint64_t p, int e)
{
    if (p == 2)                      return (seq_t)-1;
    if (p == 3)                      return (e & 1) ? (seq_t)3 : (seq_t)1;
    int mod = (int)(p % 12);
    if (mod == 1 || mod == 5)        return (seq_t)(e + 1);
    if (mod == 7 || mod == 11)       return ((e & 1) == 0) ? (seq_t)1 : (seq_t)0;
    return (seq_t)1; /* should never reach here */
}

/* main function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1, return 0 */
    if (n == 1) return (seq_t)1;

    idx_t temp   = n;
    seq_t result = (seq_t)1;

    /* factor 2 */
    int exp = 0;
    while ((temp & 1) == 0) { temp >>= 1; exp++; }
    if (exp) {
        result *= prime_power_contrib(2, exp);
        if (result == 0) return (seq_t)0;
    }

    /* factor 3 */
    exp = 0;
    while (temp % 3 == 0) { temp /= 3; exp++; }
    if (exp) {
        result *= prime_power_contrib(3, exp);
        if (result == 0) return (seq_t)0;
    }

    /* remaining factors, start from 5, skip even numbers */
    for (idx_t p = 5; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            exp = 0;
            while (temp % p == 0) { temp /= p; exp++; }
            result *= prime_power_contrib(p, exp);
            if (result == 0) return (seq_t)0;
        }
    }

    if (temp > 1)  /* remaining prime factor */
        result *= prime_power_contrib(temp, 1);

    return result;
}
