/*
 * A078868  Decimal concatenations of the quadruples (d1,d2,d3,d4) with
 *          elements in {2,4,6} for which there exists a prime p >= 5 such
 *          that the differences between the 5 consecutive primes starting
 *          with p are (d1,d2,d3,d4).
 * https://oeis.org/A078868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   idx_from_digit, make_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of all 4‑digit numbers d1d2d3d4 where
 * each di ∈ {2,4,6} and there exists a prime p ≥ 5 such that
 * the five consecutive primes p, p + d1, p + d1 + d2,
 * p + d1 + d2 + d3, p + d1 + d2 + d3 + d4 have the given gaps.
 * We build a list of all such numbers by scanning the first few
 * million primes; all occur in that range.  The list is sorted
 * increasingly, which gives the required sequence a(n). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/idx_from_digit.h"
#include "../utils/make_value.h"

/* comparison function for qsort (ascending) */
static int cmp_seq_t(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    if (cache == NULL) {
        /* ---------- 1. Generate primes up to a fixed bound ---------- */
        const idx_t LIMIT = 10000000U;   /* 10 million – enough for all patterns */
        bool *composite = (bool *)calloc(LIMIT + 1U, sizeof(bool));
        if (!composite) exit(EXIT_FAILURE);

        for (uint32_t p = 2; (uint64_t)p * p <= LIMIT; ++p) {
            if (!composite[p]) {
                for (uint64_t q = (uint64_t)p * p; q <= LIMIT; q += p) {
                    composite[q] = true;
                }
            }
        }

        /* collect primes into a vector */
        size_t primes_cap = 700000;            /* rough estimate for 10⁷ */
        size_t primes_cnt = 0;
        uint32_t *primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
        if (!primes) exit(EXIT_FAILURE);

        for (uint32_t i = 2; i <= LIMIT; ++i) {
            if (!composite[i]) {
                if (primes_cnt == primes_cap) {
                    primes_cap <<= 1;
                    uint32_t *tmp = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
                    if (!tmp) exit(EXIT_FAILURE);
                    primes = tmp;
                }
                primes[primes_cnt++] = i;
            }
        }
        free(composite);

        /* ---------- 2. Scan consecutive blocks of 5 primes ---------- */
        bool seen[81] = {false};   /* 3⁴ possibilities */
        seq_t tmp_values[81];
        size_t tmp_len = 0;

        for (size_t i = 0; i + 4 < primes_cnt; ++i) {
            if (primes[i] < 5) continue;                /* p must be ≥ 5 */

            int d1 = (int)(primes[i + 1] - primes[i]);
            int d2 = (int)(primes[i + 2] - primes[i + 1]);
            int d3 = (int)(primes[i + 3] - primes[i + 2]);
            int d4 = (int)(primes[i + 4] - primes[i + 3]);

            if (!((d1 == 2 || d1 == 4 || d1 == 6) &&
                  (d2 == 2 || d2 == 4 || d2 == 6) &&
                  (d3 == 2 || d3 == 4 || d3 == 6) &&
                  (d4 == 2 || d4 == 4 || d4 == 6)))
                continue;

            int idx = (((idx_from_digit(d1) * 3) + idx_from_digit(d2)) * 3
                       + idx_from_digit(d3)) * 3 + idx_from_digit(d4);

            if (!seen[idx]) {
                seen[idx] = true;
                tmp_values[tmp_len++] = make_value(d1, d2, d3, d4);
            }
        }
        free(primes);

        /* ---------- 3. Store sorted list in cache ---------- */
        cache_len = tmp_len;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        for (size_t i = 0; i < cache_len; ++i) cache[i] = tmp_values[i];
        qsort(cache, cache_len, sizeof(seq_t), cmp_seq_t);
    }

    if (n == 0 || (idx_t)n > cache_len) return 0;
    return cache[(idx_t)n - 1];
}
