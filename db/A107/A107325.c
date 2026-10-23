/*
 * A107325  a(n) = ceiling(A063655(n)/2).
 * https://oeis.org/A107325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil( (d1 + d2) / 2 ), where d1, d2 are the middle
 *            divisor(s) of n (for a single middle divisor, d1 = d2)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Comparator for qsort on uint64_t values. */
static int cmp_uint64(const void *a, const void *b)
{
    const uint64_t *x = (const uint64_t *)a;
    const uint64_t *y = (const uint64_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Recursive generation of all divisors from prime factors. */
static void gen_divisors(const uint64_t *primes, const int *exps,
                         int pfcnt, int idx, uint64_t cur,
                         uint64_t *divs, int *cnt)
{
    if (idx == pfcnt) {
        divs[(*cnt)++] = cur;
        return;
    }
    uint64_t p = primes[idx];
    int e = exps[idx];
    uint64_t val = cur;
    for (int i = 0; i <= e; ++i) {
        gen_divisors(primes, exps, pfcnt, idx + 1, val, divs, cnt);
        val *= p;
    }
}

/* Compute the sequence term. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* Not needed, but safe. */

    /* Factorization. */
    uint64_t m = (uint64_t)n;
    uint64_t primes[20];
    int exps[20];
    int pfcnt = 0;

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int cnt = 0;
            while (m % p == 0) {
                m /= p;
                ++cnt;
            }
            primes[pfcnt] = p;
            exps[pfcnt] = cnt;
            ++pfcnt;
        }
    }
    if (m > 1) {
        primes[pfcnt] = m;
        exps[pfcnt] = 1;
        ++pfcnt;
    }

    /* Generate all divisors.  The maximum number of divisors for n <=
       10 000 is < 200, so a fixed array of that size is safe. */
    uint64_t divisors[200];
    int divcnt = 0;
    gen_divisors(primes, exps, pfcnt, 0, 1ULL, divisors, &divcnt);

    /* Sort the divisors. */
    qsort(divisors, divcnt, sizeof(uint64_t), cmp_uint64);

    /* Compute the required middle divisor(s). */
    if (divcnt % 2 == 1) {
        /* Odd number of divisors: return the middle one. */
        return (seq_t)divisors[divcnt / 2];
    } else {
        /* Even number: ceil of average of the two middle divisors. */
        uint64_t a = divisors[divcnt / 2 - 1];
        uint64_t b = divisors[divcnt / 2];
        uint64_t sum = a + b;
        uint64_t result = (sum + 1) / 2;  /* ceil division by 2 */
        return (seq_t)result;
    }
}
