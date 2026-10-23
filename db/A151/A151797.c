/*
 * A151797  Numbers which belong to just one of the two sequences A030229,
 *          A161918.
 * https://oeis.org/A151797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers which belong to just one of the two sequences A030229 and A161918.
   This implementation generates all products of four distinct primes (which
   comprise A030229) and returns 1, 8, followed by the sorted list of those
   products.  These are the terms that belong to exactly one of the two
   sequences. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes.h"

/* comparison function for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* cached sorted list of products of 4 distinct primes */
static seq_t *s4_seq = NULL;
static size_t s4_len = 0;   /* number of elements in s4_seq */

/* ensure that s4_seq contains at least 'needed' elements (needed >= 1) */
static void ensure_s4(size_t needed)
{
    if (s4_len >= needed) return;

    uint64_t limit = 100;
    size_t primes_cnt = 0;
    idx_t *primes = NULL;

    /* Find minimal limit such that C(pi(limit),4) >= needed */
    while (1) {
        build_primes(limit, &primes, &primes_cnt);
        if (primes_cnt < 4) {
            free(primes);
            limit *= 2;
            continue;
        }
        size_t combos = primes_cnt * (primes_cnt - 1) *
                        (primes_cnt - 2) * (primes_cnt - 3) / 24;
        free(primes);
        if (combos >= needed) break;
        limit *= 2;
    }

    /* Build primes array for the final limit */
    build_primes(limit, &primes, &primes_cnt);

    size_t combos = primes_cnt * (primes_cnt - 1) *
                    (primes_cnt - 2) * (primes_cnt - 3) / 24;

    seq_t *tmp = (seq_t *)malloc(combos * sizeof(seq_t));
    if (!tmp) { free(primes); return; }

    size_t pos = 0;
    for (size_t i = 0; i + 3 < primes_cnt; ++i) {
        for (size_t j = i + 1; j + 2 < primes_cnt; ++j) {
            for (size_t k = j + 1; k + 1 < primes_cnt; ++k) {
                for (size_t l = k + 1; l < primes_cnt; ++l) {
                    seq_t prod = (seq_t)primes[i];
                    prod *= primes[j];
                    prod *= primes[k];
                    prod *= primes[l];
                    tmp[pos++] = prod;
                }
            }
        }
    }

    qsort(tmp, combos, sizeof(seq_t), cmp_seq_t);

    /* Replace old cache with new sorted array */
    free(s4_seq);
    s4_seq = tmp;
    s4_len = combos;

    free(primes);
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)8;
    idx_t idx = n - 3;              /* zero‑based index in the prime-product list */
    ensure_s4((size_t)idx + 1);
    return s4_seq[(size_t)idx];
}
