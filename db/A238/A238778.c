/*
 * A238778  Sum of all primes p such that 2n - p is also a prime.
 * https://oeis.org/A238778
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ p : p prime, 2n‑p prime. (Offset 2)
 * Range:     u8 n <= 26, u16 n <= 569, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* Comparator for bsearch on idx_t values */
static int cmp_idx(const void *a, const void *b) {
    idx_t ai = *(const idx_t *)a;
    idx_t bi = *(const idx_t *)b;
    return (ai > bi) - (ai < bi);
}

/* Test whether x is in the sorted prime array `primes` */
static bool in_primes(const idx_t *primes, size_t cnt, idx_t x) {
    return bsearch(&x, primes, cnt, sizeof(idx_t), cmp_idx) != NULL;
}

seq_t a(idx_t n) {
    if (n < 2) return 0;               /* offset starts at 2 */
    const idx_t limit = 2 * n;          /* 2n */

    size_t  primes_cnt = 0;
    idx_t  *primes = NULL;
    build_primes_upto(limit, &primes, &primes_cnt);  /* primes[0 .. primes_cnt-1] sorted */

    seq_t sum = 0;
    for (size_t i = 0; i < primes_cnt && primes[i] <= limit; ++i) {
        idx_t p = primes[i];
        idx_t q = limit - p;
        if (in_primes(primes, primes_cnt, q))
            sum += (seq_t)p;
    }

    free(primes);
    return sum;
}
