/*
 * A091010  Numbers having no divisor d such that also d-x and d+x are
 *          divisors for some x.
 * https://oeis.org/A091010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A091010: numbers having no divisor d such that also d‑x and d+x are divisors for some x. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Comparator for qsort/bsearch on uint64_t */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

/* Binary search for target in sorted array of uint64_t */
static bool contains_uint64(uint64_t *arr, size_t n, uint64_t target)
{
    return bsearch(&target, arr, n, sizeof(uint64_t), cmp_uint64) != NULL;
}

/* Test whether the integer m has a 3‑term arithmetic progression in its divisor set.
   The divisor list is returned sorted in `divs` and `cnt` contains its length. */
static bool has_arith_progression(uint64_t m, uint64_t *divs, size_t cnt)
{
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t center = divs[i];
        for (size_t j = 0; j < i; ++j) {
            uint64_t diff   = center - divs[j];
            uint64_t target = center + diff;
            if (target > divs[cnt - 1])
                continue;
            if (contains_uint64(divs, cnt, target))
                return true;           /* arithmetic triple found */
        }
    }
    return false;                       /* no triple */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Static state */
    static seq_t *seq = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;
    static uint64_t next_candidate = 1;

    /* Grow the stored sequence array */
    if (seq_len == seq_cap) {
        size_t new_cap = (seq_cap == 0) ? 16 : seq_cap * 2;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }

    /* Generate terms until we have at least n of them */
    while (seq_len < n) {
        uint64_t m = next_candidate++;

        /* Build the divisor list of m */
        uint64_t divs[128];
        size_t cnt = 0;
        for (uint64_t d = 1; d * d <= m; ++d) {
            if (m % d == 0) {
                divs[cnt++] = d;
                if (d != m / d)
                    divs[cnt++] = m / d;
            }
        }
        qsort(divs, cnt, sizeof(uint64_t), cmp_uint64);

        if (!has_arith_progression(m, divs, cnt))
            seq[seq_len++] = (seq_t)m;     /* accept the candidate */
    }

    return seq[n - 1];
}
