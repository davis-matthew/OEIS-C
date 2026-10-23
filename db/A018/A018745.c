/*
 * A018745  Divisors of 962.
 * https://oeis.org/A018745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the n-th divisor of 962 in ascending order
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Cached divisors of 962 */
static seq_t *cached_divs = NULL;
static size_t cached_cnt = 0;

/* Compute and cache the divisors of 962 */
static void init_divisors(void) {
    if (cached_cnt) return;                       /* already done */
    const uint64_t N = 962ULL;
    uint64_t temp = N;
    uint64_t primes[20];
    size_t pc = 0;

    if (temp % 2ULL == 0) {                      /* factor 2 */
        primes[pc++] = 2ULL;
        while (temp % 2ULL == 0) temp /= 2ULL;
    }
    for (uint64_t p = 3ULL; p * p <= temp; p += 2ULL) {
        if (temp % p == 0) {
            primes[pc++] = p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1ULL) primes[pc++] = temp;        /* remaining prime */

    cached_cnt = (size_t)1 << pc;                /* 2^pc divisors */

    cached_divs = (seq_t*)malloc(sizeof(seq_t) * cached_cnt);
    if (!cached_divs) return;                    /* malloc failure */

    /* Generate all products for every subset of primes */
    for (size_t mask = 0; mask < cached_cnt; ++mask) {
        seq_t val = (seq_t)1;
        for (size_t i = 0; i < pc; ++i)
            if (mask & (1ULL << i))
                val = val * (seq_t)primes[i];
        cached_divs[mask] = val;
    }

    /* Simple insertion sort (small array) */
    for (size_t i = 1; i < cached_cnt; ++i) {
        seq_t key = cached_divs[i];
        size_t j = i;
        while (j > 0 && cached_divs[j - 1] > key) {
            cached_divs[j] = cached_divs[j - 1];
            --j;
        }
        cached_divs[j] = key;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    init_divisors();
    if (n <= cached_cnt) return cached_divs[n - 1];
    return (seq_t)0;      /* no such divisor */
}
