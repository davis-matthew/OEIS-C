/*
 * A064948  a(n) = Sum_{i|n, j|n} max(i,j).
 * https://oeis.org/A064948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i|n, j|n} max(i,j) = Sum_{m=1..tau(n)} (2*m-1)*d_m
 *            where d_m are the divisors of n in increasing order
 * Range:     u8 n <= 17, u16 n <= 539, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Comparator for qsort on seq_t */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*b < *a) return 1;
    return 0;
}

/* Main function */
seq_t a(idx_t n)
{
    /* Handle n=1 directly */
    if (n == 1) {
        return (seq_t)1;
    }

    /* 1) Factor n and collect primes and exponents */
    idx_t m = n;
    idx_t prime[64];
    idx_t exp[64];
    size_t cnt = 0;

    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            prime[cnt] = p;
            exp[cnt] = e;
            ++cnt;
        }
    }
    if (m > 1) {
        prime[cnt] = m;
        exp[cnt] = 1;
        ++cnt;
    }

    /* 2) Compute tau(n) = product (e_i + 1) */
    seq_t tau = 1;
    for (size_t i = 0; i < cnt; ++i) {
        tau *= (seq_t)(exp[i] + 1);
    }

    /* 3) Generate all divisors into array divs[0..tau-1] */
    if (tau == 0) { /* should not happen */
        return (seq_t)0;
    }

    seq_t *divs = (seq_t *)malloc((size_t)tau * sizeof(seq_t));
    if (!divs) {
        return (seq_t)0; /* out of memory */
    }

    size_t pos = 0;
    divs[pos++] = (seq_t)1;           /* start with divisor 1 */

    for (size_t idx = 0; idx < cnt; ++idx) {
        seq_t p = (seq_t)prime[idx];
        idx_t e = exp[idx];
        size_t currentSize = pos;
        seq_t pk = (seq_t)1;
        for (idx_t k = 1; k <= e; ++k) {
            pk *= p;
            for (size_t j = 0; j < currentSize; ++j) {
                divs[pos++] = divs[j] * pk;
            }
        }
    }

    /* 4) Sort divisors in increasing order */
    qsort(divs, (size_t)tau, sizeof(seq_t), cmp_seq_t);

    /* 5) Compute the required sum */
    seq_t sum = (seq_t)0;
    for (seq_t idx = 0; idx < tau; ++idx) {
        seq_t mult = (seq_t)(2 * (idx + 1) - 1);   /* (2*m-1) */
        sum += divs[(size_t)idx] * mult;
    }

    free(divs);
    return sum;
}
