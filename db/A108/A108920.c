/*
 * A108920  Number of positive integers k>n such that n+k divides n^2+k^2.
 * https://oeis.org/A108920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of 2*n^2 such that d < n
 * Range:     u8 n <= 15, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   count_divisors_less
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This follows from the equivalence n+k | n^2+k^2 ⇔ n+k | 2n^2 and k>n ⇔ d=n+k>2n. */

#include <stdint.h>
#include "../utils/count_divisors_less.h"

seq_t a(idx_t n)
{
    /* Factorise n */
    idx_t temp = n;
    static const int MAXP = 64;
    idx_t primes[MAXP];
    idx_t exps[MAXP];
    int pcnt = 0;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt] = e;
            ++pcnt;
        }
    }
    if (temp > 1) {
        primes[pcnt] = temp;
        exps[pcnt] = 1;
        ++pcnt;
    }

    /* Build factorisation of 2*n^2 */
    idx_t primes2[MAXP];
    idx_t exps2[MAXP];
    int pcnt2 = 0;
    /* exponent of 2 */
    idx_t e2 = 0;
    for (int i = 0; i < pcnt; ++i)
        if (primes[i] == 2) { e2 = exps[i]; break; }
    primes2[pcnt2] = 2;
    exps2[pcnt2] = 1 + 2 * e2;   /* exponent of 2 in 2*n^2 */
    ++pcnt2;
    for (int i = 0; i < pcnt; ++i) {
        if (primes[i] == 2) continue;
        primes2[pcnt2] = primes[i];
        exps2[pcnt2] = 2 * exps[i];   /* exponent doubled */
        ++pcnt2;
    }

    seq_t cntLess = 0;
    count_divisors_less(primes2, exps2, pcnt2, 0, (seq_t)1, n, &cntLess);
    return cntLess;
}
