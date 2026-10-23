/*
 * A018505  Divisors of 532.
 * https://oeis.org/A018505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 532, sorted in increasing order
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

static int seq_compare(const void *p1, const void *p2) {
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*x > *y) return  1;
    return 0;
}

/* Generate all divisors of 532 and store them sorted in `out`. */
static void compute_divisors(seq_t out[], size_t *len) {
    const uint64_t T = 532ULL;
    uint64_t primes[32];
    int exps[32];
    int pcount = 0;
    uint64_t t = T;

    /* Trial‑division factorisation of 532. */
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            int e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            primes[pcount]   = p;
            exps[pcount]     = e;
            ++pcount;
        }
    }
    if (t > 1) {
        primes[pcount] = t;
        exps[pcount]   = 1;
        ++pcount;
    }

    /* Generate divisors. */
    out[0] = 1;
    *len  = 1;
    for (int i = 0; i < pcount; ++i) {
        uint64_t p = primes[i];
        int e     = exps[i];
        size_t old_len = *len;
        for (size_t j = 0; j < old_len; ++j) {
            uint64_t base = (uint64_t)out[j];
            uint64_t val  = base;
            for (int k = 1; k <= e; ++k) {
                val *= p;
                out[(*len)++] = (seq_t)val;
            }
        }
    }

    /* Sort the divisors. */
    qsort(out, *len, sizeof(seq_t), seq_compare);
}

seq_t a(idx_t n) {
    static seq_t divisors[64];
    static size_t div_count = 0;

    if (div_count == 0) {
        compute_divisors(divisors, &div_count);
    }

    if (n >= 1 && (size_t)n <= div_count) {
        return divisors[n - 1];
    }
    /* For indices beyond the list, return 0 (undefined for OEIS). */
    return (seq_t)0;
}
