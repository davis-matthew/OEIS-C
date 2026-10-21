/*
 * A064212  a(n) = sigma(n) + usigma(n) = A000203(n) + A034448(n).
 * https://oeis.org/A064212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) + usigma(n) where sigma is the sum of all
 *            divisors of n and usigma is the sum of unitary divisors of n
 *            (unitary divisor d of n satisfies gcd(d, n/d) = 1)
 * Range:     u8 n <= 59, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/*--- Sieve for smallest prime factors ---*/
static uint32_t *spf = NULL;        /* 0 = not set, 1 = special for n=1 */
static size_t   spf_size = 0;       /* number of elements in spf array */

/* Build spf[0..limit] (relaxes old allocation if needed) */
static void init_sieve(idx_t limit)
{
    if (spf != NULL && spf_size > (size_t)limit)
        return;                         /* already sufficient */

    /* Free old array and allocate new one */
    free(spf);
    spf_size = (size_t)limit + 1;
    spf = (uint32_t *)malloc(spf_size * sizeof(uint32_t));
    if (spf == NULL)                    /* allocation failed */
        return;

    for (size_t i = 0; i < spf_size; ++i)
        spf[i] = 0;
    spf[0] = 0;
    spf[1] = 1;

    /* Simple sieve to fill spf */
    for (idx_t i = 2; i <= limit; ++i) {
        size_t idx = (size_t)i;
        if (spf[idx] == 0) {
            spf[idx] = (uint32_t)i;
            unsigned long long ii = (unsigned long long)i * i;
            if (ii > limit)
                continue;
            for (unsigned long long j = ii; j <= limit; j += i) {
                size_t jdx = (size_t)j;
                if (spf[jdx] == 0)
                    spf[jdx] = (uint32_t)i;
            }
        }
    }
}

/*--- Main sequence function ---*/
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)2;                /* sigma(1)=1, usigma(1)=1 */

    init_sieve(n);                       /* ensure spf up to n */

    idx_t temp = n;
    seq_t sigma = 1;
    seq_t unitary = 1;

    while (temp > 1) {
        uint32_t p = spf[(size_t)temp];
        /* count exponent of p in n */
        idx_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            e++;
        }

        /* compute p^e and 1+p+...+p^e */
        seq_t pow = 1;   /* p^e */
        seq_t sum = 1;   /* 1 + p + ... + p^e */
        for (idx_t i = 1; i <= e; ++i) {
            pow *= (seq_t)p;
            sum += pow;
        }

        sigma *= sum;
        unitary *= (pow + (seq_t)1);
    }

    return sigma + unitary;
}
