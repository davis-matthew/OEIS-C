/*
 * A062327  Number of divisors of n over the Gaussian integers.
 * https://oeis.org/A062327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, mult
 * Formula:   a(n) = ∏_{p^e||n} g(p,e) , with * g(2,e) = 2*e+1 * g(p,e) = e+1
 *            if p%4==3 * g(p,e) = (e+1)^2 if p%4==1 * This is the number of
 *            divisors of n in the Gaussian integers, * distinct up to
 *            multiplication by the units {±1,±i}
 * Range:     u8 n <= 3119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;   /* a(1) = 1 */

    uint64_t tn = (uint64_t)n;
    seq_t   result = (seq_t)1;

    /* Handle factor 2 separately. */
    uint64_t exp2 = 0;
    while (tn % 2 == 0) {
        tn /= 2;
        exp2++;
    }
    if (exp2 > 0) {
        seq_t term = (seq_t)(2 * exp2 + 1);
        result *= term;
    }

    /* Trial division for odd primes. */
    uint64_t p = 3;
    for (; p * p <= tn; p += 2) {
        if (tn % p == 0) {
            uint64_t exp = 0;
            while (tn % p == 0) {
                tn /= p;
                exp++;
            }
            seq_t term;
            if (p % 4 == 3) {
                term = (seq_t)(exp + 1);
            } else { /* p % 4 == 1 */
                seq_t e1 = (seq_t)(exp + 1);
                term = e1 * e1;
            }
            result *= term;
        }
    }

    /* If remaining tn is >1, it is a prime > sqrt(original n). */
    if (tn > 1) {
        seq_t term;
        if (tn % 2 == 0) {          /* Should not happen, but for safety */
            term = (seq_t)(2 * (uint64_t)1 + 1);
        } else if (tn % 4 == 3) {
            term = (seq_t)(1 + 1);
        } else {                /* tn % 4 == 1 */
            seq_t e1 = (seq_t)2;
            term = e1 * e1;
        }
        result *= term;
    }

    return result;
}
