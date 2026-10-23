/*
 * A018771  Divisors of 1005.
 * https://oeis.org/A018771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Divisors of 1005.
   a(n) = nth smallest divisor of 1005 (a(1)=1,a(8)=1005). */

#include <stdint.h>
#include <stdlib.h>

/* Helper: compare two uint64_t values for qsort. */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t x = *(const uint64_t *)a;
    uint64_t y = *(const uint64_t *)b;
    return (x < y) ? -1 : (x > y) ? 1 : 0;
}

seq_t a(idx_t n)
{
    /* 0‑based index for array access */
    if (n < 1) return 0;

    const uint64_t number = 1005;
    static uint64_t *divisors = NULL;      /* array of divisors */
    static size_t  count      = 0;         /* number of divisors */

    if (divisors == NULL) {
        /* Factorize 1005 (small constant) */
        uint64_t primes[8];
        uint64_t exps  [8];
        size_t   pfcnt = 0;
        uint64_t m = number;
        for (uint64_t p = 2; p * p <= m; ++p) {
            if (m % p == 0) {
                uint64_t e = 0;
                while (m % p == 0) { m /= p; ++e; }
                primes[pfcnt] = p;
                exps  [pfcnt] = e;
                ++pfcnt;
            }
        }
        if (m > 1) {                       /* remaining prime factor */
            primes[pfcnt] = m;
            exps  [pfcnt] = 1;
            ++pfcnt;
        }

        /* Total number of divisors = Π (ei + 1) */
        count = 1;
        for (size_t i = 0; i < pfcnt; ++i) count *= (exps[i] + 1);

        /* Allocate and generate divisors */
        divisors = (uint64_t *)malloc(count * sizeof(uint64_t));
        divisors[0] = 1;
        size_t cur = 1;

        for (size_t i = 0; i < pfcnt; ++i) {
            size_t prev = cur;
            uint64_t power = 1;
            for (uint64_t e = 1; e <= exps[i]; ++e) {
                power *= primes[i];
                for (size_t j = 0; j < prev; ++j) {
                    divisors[cur++] = divisors[j] * power;
                }
            }
        }
        /* Sort the divisor list (small list, qsort is fine) */
        qsort(divisors, count, sizeof(uint64_t), cmp_uint64);
    }

    /* Return the n‑th divisor or 0 if n exceeds the list */
    if ((size_t)n <= count)
        return (seq_t)divisors[n - 1];
    return 0;
}
