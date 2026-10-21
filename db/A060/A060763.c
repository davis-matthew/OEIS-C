/*
 * A060763  Number of distinct differences between consecutive divisors
 *          (ordered by increasing magnitude) of n which are not also divisors
 *          of n.
 * https://oeis.org/A060763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct differences between consecutive
 *            divisors of n (ordered by increasing magnitude) which are not
 *            also divisors of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   rec_generate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/rec_generate.h"

static int cmp_idx(const void *a, const void *b) {
    idx_t v1 = *(const idx_t *)a;
    idx_t v2 = *(const idx_t *)b;
    return (v1 < v2) ? -1 : (v1 > v2) ? 1 : 0;
}

seq_t a(idx_t n) {
    idx_t orig = n;
    if (n == 0) return (seq_t)0; /* undefined but safe */

    /* factor n */
    idx_t primes[64];
    int exps[64];
    size_t pcount = 0;

    if (n % 2 == 0) {
        int e = 0;
        while (n % 2 == 0) {
            n /= 2;
            ++e;
        }
        primes[pcount] = 2;
        exps[pcount] = e;
        ++pcount;
    }
    for (idx_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) {
            int e = 0;
            while (n % d == 0) {
                n /= d;
                ++e;
            }
            primes[pcount] = d;
            exps[pcount] = e;
            ++pcount;
        }
    }
    if (n > 1) {
        primes[pcount] = n;
        exps[pcount] = 1;
        ++pcount;
    }

    /* generate all divisors */
    static const size_t MAX_DIV = 8192;
    idx_t divs[MAX_DIV];
    size_t divcnt = 0;
    rec_generate(1, 0, primes, exps, pcount, divs, &divcnt);

    /* sort divisors */
    qsort(divs, divcnt, sizeof(idx_t), cmp_idx);

    /* count distinct non‑divisor differences */
    seq_t count = 0;
    static const size_t MAX_DIFF = MAX_DIV;
    idx_t diffs[MAX_DIFF];
    size_t diffcnt = 0;

    for (size_t i = 0; i + 1 < divcnt; ++i) {
        idx_t diff = divs[i + 1] - divs[i];
        bool seen = false;
        for (size_t j = 0; j < diffcnt; ++j) {
            if (diffs[j] == diff) {
                seen = true;
                break;
            }
        }
        if (!seen) {
            diffs[diffcnt++] = diff;
            if (orig % diff != 0) {
                ++count;
            }
        }
    }
    return count;
}
