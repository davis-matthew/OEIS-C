/*
 * A018525  Divisors of 572.
 * https://oeis.org/A018525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 572. a(n) is the nth divisor of 572 in ascending order. */

#include <stdlib.h>

static int div_cmp(const void *p, const void *q) {
    const seq_t *x = (const seq_t *)p;
    const seq_t *y = (const seq_t *)q;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

seq_t a(idx_t n) {
    /* static cache: divisors of 572 */
    static seq_t divs[128];
    static idx_t div_count = 0;

    if (div_count == 0) {
        /* factor 572 = 2^2 * 11 * 13 */
        uint64_t rem = 572;
        uint64_t primes[10];
        int exps[10];
        int pcnt = 0;

        for (uint64_t p = 2; p * p <= rem; ++p) {
            if (rem % p == 0) {
                int cnt = 0;
                while (rem % p == 0) {
                    rem /= p;
                    ++cnt;
                }
                primes[pcnt] = p;
                exps[pcnt] = cnt;
                ++pcnt;
            }
        }
        if (rem > 1) {
            primes[pcnt] = rem;
            exps[pcnt] = 1;
            ++pcnt;
        }

        /* generate divisors */
        divs[0] = (seq_t)1;
        div_count = 1;

        for (int i = 0; i < pcnt; ++i) {
            uint64_t p = primes[i];
            int e = exps[i];
            idx_t current = div_count;
            for (idx_t j = 0; j < current; ++j) {
                seq_t base = divs[j];
                seq_t mul = base;
                for (int k = 1; k <= e; ++k) {
                    mul = mul * (seq_t)p;
                    divs[div_count++] = mul;
                }
            }
        }
        /* sort divisors */
        qsort(divs, div_count, sizeof(seq_t), div_cmp);
    }

    if (n <= div_count)
        return divs[(idx_t)n - 1];
    else
        return (seq_t)0; /* undefined for n beyond the list */
}
