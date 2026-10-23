/*
 * A197881  Number of times n occurs in A197863.
 * https://oeis.org/A197881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   multiplicative function * a(1) = 1 * a(p^e) = 0 if e = 1 * 2 if
 *            e = 2 * 1 if e ≥ 3 * for all n ≥ 1 * This implements a(n) = 0 if
 *            n has any prime factor to the first power; * otherwise a(n) =
 *            2^k where k = #{primes p : v_p(n) = 2}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n <= 1)                      /* a(1) = 1, a(0) never used */
        return (seq_t)1;

    uint64_t m = n;                  /* factorisation work in uint64_t */
    unsigned int cnt_sq = 0;         /* number of prime factors with exponent 2 */

    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p != 0)
            continue;

        /* count exponent of prime p in n */
        unsigned int exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }

        if (exp == 1)                 /* presence of a first‑power prime factor -> 0 */
            return (seq_t)0;
        if (exp == 2)                 /* contributes a factor 2 in the product */
            ++cnt_sq;
        /* exp ≥ 3 contributes 1 → no change to cnt_sq */
    }

    /* any remaining m > 1 is a prime factor with exponent 1 */
    if (m > 1)
        return (seq_t)0;

    /* compute 2^cnt_sq */
    seq_t result = (seq_t)1;
    for (unsigned int i = 0; i < cnt_sq; ++i)
        result *= (seq_t)2;

    return result;
}
