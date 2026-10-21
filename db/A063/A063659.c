/*
 * A063659  The number of integers m in [1..n] for which gcd(m,n) is not
 *          divisible by a square greater than 1.
 * https://oeis.org/A063659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   a(n) = product_{p^e||n} (p if e = 1, * p^e - p^(e-2) if e > 1)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1;  for n ≥ 1, a(n) ≤ n.  The function is fully
 * recomputing the value from the prime factorisation of n. */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n<1, but safe guard */

    seq_t result = 1;
    idx_t m = n;
    int e = 0;

    /* factor out powers of 2 */
    while ((m & 1) == 0) {
        ++e;
        m >>= 1;
    }
    if (e > 0) {
        seq_t term;
        if (e == 1)
            term = 2;          /* p == 2, e == 1 */
        else {
            seq_t pow = 1;
            for (int i = 0; i < e; ++i)
                pow *= (seq_t)2;      /* compute 2^e */
            seq_t pow_minus2 = pow / ((seq_t)2 * (seq_t)2);  /* 2^(e-2) */
            term = pow - pow_minus2;                         /* 2^e - 2^(e-2) */
        }
        result *= term;
    }

    /* factor out odd primes */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            e = 0;
            while (m % p == 0) {
                ++e;
                m /= p;
            }

            seq_t term;
            if (e == 1)
                term = (seq_t)p;
            else {
                seq_t pow = 1;
                for (int i = 0; i < e; ++i)
                    pow *= (seq_t)p;          /* compute p^e */
                seq_t pow_minus2 = pow / ((seq_t)p * (seq_t)p);   /* p^(e-2) */
                term = pow - pow_minus2;                     /* p^e - p^(e-2) */
            }
            result *= term;
        }
    }

    /* any remaining factor is a prime with exponent 1 */
    if (m > 1)
        result *= (seq_t)m;

    return result;
}
