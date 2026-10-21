/*
 * A062363  a(n) = Sum_{d|n} d!.
 * https://oeis.org/A062363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{d|n} d!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

static size_t divcnt;
static uint64_t divs[4096];

static void gen_divs(uint64_t cur, size_t idx, size_t pcnt,
                     const uint64_t *pr, const int *ep)
{
    if (idx == pcnt) {
        divs[divcnt++] = cur;
        return;
    }
    uint64_t mult = 1;
    for (int e = 0; e <= ep[idx]; ++e) {
        gen_divs(cur * mult, idx + 1, pcnt, pr, ep);
        mult *= pr[idx];
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    uint64_t N = (uint64_t)n;

    /* Factorise N */
    uint64_t primes[20];
    int exps[20];
    size_t pcnt = 0;
    uint64_t m = N;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt] = e;
            ++pcnt;
        }
    }
    if (m > 1) {
        primes[pcnt] = m;
        exps[pcnt] = 1;
        ++pcnt;
    }

    /* Generate all divisors of N */
    divcnt = 0;
    gen_divs(1, 0, pcnt, primes, exps);

    /* Sort divisors */
    qsort(divs, divcnt, sizeof(uint64_t), cmp_uint64);

    /* Sum factorials of divisors */
    seq_t sum = 0;
    seq_t fact = 1;          /* fact = curfactor! */
    uint64_t curfactor = 1; /* current key of factorial */

    for (size_t i = 0; i < divcnt; ++i) {
        uint64_t d = divs[i];
        while (curfactor < d) {
            fact *= (seq_t)(curfactor + 1);
            ++curfactor;
        }
        sum += fact;
    }
    return sum;
}
