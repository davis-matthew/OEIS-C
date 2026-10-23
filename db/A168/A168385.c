/*
 * A168385  Numbers of the form |a^b - c^d| where a, b, c and d are the first
 *          4 primes.
 * https://oeis.org/A168385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = nth smallest |p_i^p_j - p_k^p_l| where {p_i} = {2,3,5,7}
 *            with the four primes used exactly once in each term. There are
 *            12 distinct values; a(n)=0 for n>12
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

static seq_t pow_small(seq_t base, idx_t exp)
{
    seq_t r = 1;
    while (exp > 0) {
        r *= base;
        exp--;
    }
    return r;
}

static int cmp_seq_t(const void *a, const void *b)
{
    seq_t va = *(seq_t *)a;
    seq_t vb = *(seq_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

static void init_sequence(void)
{
    static const idx_t primes[4] = {2, 3, 5, 7};
    seq_t all[24];
    size_t allcnt = 0;

    /* Enumerate all permutations of the four primes */
    for (idx_t i = 0; i < 4; ++i) {
        for (idx_t j = 0; j < 4; ++j) {
            if (j == i) continue;
            for (idx_t k = 0; k < 4; ++k) {
                if (k == i || k == j) continue;
                for (idx_t l = 0; l < 4; ++l) {
                    if (l == i || l == j || l == k) continue;
                    seq_t v1 = pow_small((seq_t)primes[i], (idx_t)primes[j]);
                    seq_t v2 = pow_small((seq_t)primes[k], (idx_t)primes[l]);
                    all[allcnt++] = (v1 > v2) ? v1 - v2 : v2 - v1;
                }
            }
        }
    }

    qsort(all, allcnt, sizeof(seq_t), cmp_seq_t);

    static seq_t values[12];
    static size_t valcnt = 0;
    values[0] = all[0];
    valcnt = 1;
    for (size_t m = 1; m < allcnt; ++m) {
        if (all[m] != values[valcnt - 1]) {
            values[valcnt] = all[m];
            valcnt++;
        }
    }

    /* Store in static global variables */
    if (!valcnt) return;
    static seq_t *array = NULL;
    array = (seq_t *)malloc(valcnt * sizeof(seq_t));
    for (size_t t = 0; t < valcnt; ++t) array[t] = values[t];
    static seq_t *cache = NULL;
    static size_t cachecnt = 0;
    cache = array;
    cachecnt = valcnt;
    static bool built = true;
    (void)built; /* suppress unused variable */
}

seq_t a(idx_t n)
{
    /* Lazy initialization */
    static bool built = false;
    static seq_t *cache = NULL;
    static size_t cachecnt = 0;
    if (!built) {
        const idx_t primes[4] = {2, 3, 5, 7};
        seq_t all[24];
        size_t allcnt = 0;
        for (idx_t i = 0; i < 4; ++i)
            for (idx_t j = 0; j < 4; ++j) {
                if (j == i) continue;
                for (idx_t k = 0; k < 4; ++k) {
                    if (k == i || k == j) continue;
                    for (idx_t l = 0; l < 4; ++l) {
                        if (l == i || l == j || l == k) continue;
                        seq_t v1 = pow_small((seq_t)primes[i], (idx_t)primes[j]);
                        seq_t v2 = pow_small((seq_t)primes[k], (idx_t)primes[l]);
                        all[allcnt++] = (v1 > v2) ? v1 - v2 : v2 - v1;
                    }
                }
            }
        qsort(all, allcnt, sizeof(seq_t), cmp_seq_t);
        static seq_t values[12];
        size_t valcnt = 0;
        values[0] = all[0];
        valcnt = 1;
        for (size_t m = 1; m < allcnt; ++m)
            if (all[m] != values[valcnt - 1])
                values[valcnt++] = all[m];
        cache = (seq_t *)malloc(valcnt * sizeof(seq_t));
        for (size_t t = 0; t < valcnt; ++t) cache[t] = values[t];
        cachecnt = valcnt;
        built = true;
    }

    if (n == 0 || n > cachecnt) return 0;
    return cache[n - 1];
}
