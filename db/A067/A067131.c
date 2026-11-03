/*
 * A067131  Number of elements in the largest set of divisors of n which are
 *          in arithmetic progression.
 * https://oeis.org/A067131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the maximum length of an arithmetic progression *
 *            contained in the set of positive divisors of n. * The
 *            implementation enumerates all divisors of n, sorts them, * and
 *            for each pair of divisors (d1,d2) tries to extend the *
 *            arithmetic progression starting at d1 with difference d2-d1. *
 *            Com...
 * Range:     u8 n <= 1563, u16 n <= 65536, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 119 terms through n = 9764 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Binary search – assume arr is sorted. */
static bool exists_in_arr(const seq_t *arr, size_t len, seq_t val)
{
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        seq_t v = arr[mid];
        if (v == val) return true;
        if (v < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return false;
}

/* Compare function for qsort */
static int cmp_seq_t(const void *a, const void *b)
{
    seq_t v1 = *(const seq_t *)a;
    seq_t v2 = *(const seq_t *)b;
    return (v1 < v2) ? -1 : (v1 > v2) ? 1 : 0;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Edge case: n = 0 not used, but guard. */
    if (n == 0) return 0;

    /* Dynamic array for divisors */
    size_t cap = 64;
    seq_t *divs = mem_new(seq_t, cap);
    if (!divs) return 0;           /* failed allocation – unlikely */
    size_t cnt = 0;

    /* Enumerate divisors */
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            if (cnt == cap) {
                cap *= 2;
                divs = mem_resize(seq_t, divs, cap);
                if (!divs) return 0;
            }
            divs[cnt++] = (seq_t)i;
            if (i != n / i) {
                if (cnt == cap) {
                    cap *= 2;
                    divs = mem_resize(seq_t, divs, cap);
                    if (!divs) return 0;
                }
                divs[cnt++] = (seq_t)(n / i);
            }
        }
    }

    /* Sort divisors */
    mem_qsort(divs, cnt, sizeof(seq_t), cmp_seq_t);

    /* Find longest arithmetic progression among divisors */
    seq_t best = 1;   /* at least one divisor */
    for (size_t i = 0; i < cnt; ++i) {
        for (size_t j = i + 1; j < cnt; ++j) {
            seq_t diff = divs[j] - divs[i];
            if (diff == 0) continue;
            seq_t len = 2;
            seq_t term = divs[j];
            while (exists_in_arr(divs, cnt, term + diff)) {
                term += diff;
                ++len;
            }
            if (len > best) best = len;
        }
    }

    mem_free(divs);
    return best;
}
