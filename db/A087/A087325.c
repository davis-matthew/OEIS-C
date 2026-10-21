/*
 * A087325  Numbers k such that k and its 10's complement both have the same
 *          prime signature.
 * https://oeis.org/A087325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is computed by searching integers whose prime signature equals that
 * of its 10's complement. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/factor_u64.h"

/* Return true if a and b have the same multiset of prime exponents. */
static bool same_signature_uint64(uint64_t a, uint64_t b)
{
    size_t cnt_a, cnt_b;
    uint64_t pr_a[64], ex_a[64];
    uint64_t pr_b[64], ex_b[64];

    factor_u64(a, pr_a, ex_a, &cnt_a);
    factor_u64(b, pr_b, ex_b, &cnt_b);

    if (cnt_a != cnt_b)
        return false;

    /* Sort the exponent arrays. The arrays are tiny (≤16 elements). */
    uint64_t tmp_a[64], tmp_b[64];
    memcpy(tmp_a, ex_a, cnt_a * sizeof(uint64_t));
    memcpy(tmp_b, ex_b, cnt_b * sizeof(uint64_t));

    for (size_t i = 1; i < cnt_a; ++i) {
        uint64_t key = tmp_a[i];
        size_t j = i;
        while (j > 0 && tmp_a[j - 1] > key) {
            tmp_a[j] = tmp_a[j - 1];
            --j;
        }
        tmp_a[j] = key;
    }
    for (size_t i = 1; i < cnt_b; ++i) {
        uint64_t key = tmp_b[i];
        size_t j = i;
        while (j > 0 && tmp_b[j - 1] > key) {
            tmp_b[j] = tmp_b[j - 1];
            --j;
        }
        tmp_b[j] = key;
    }

    for (size_t i = 0; i < cnt_a; ++i)
        if (tmp_a[i] != tmp_b[i])
            return false;

    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Static cache of discovered terms. */
    static seq_t *cache = NULL;
    static size_t cap = 0, size = 0;
    static uint64_t k_current = 1;  /* next integer to test */

    if (n == 0)
        return 0;

    /* Grow cache if first call. */
    if (cap == 0) {
        cap = 128;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n terms. */
    while (size < n) {
        uint64_t k = k_current;

        /* Compute number of decimal digits of k. */
        uint64_t tmp = k;
        int digits = 0;
        do {
            ++digits;
            tmp /= 10;
        } while (tmp > 0);

        /* Compute 10^digits. */
        uint64_t ten_pow = 1;
        for (int i = 0; i < digits; ++i)
            ten_pow *= 10;

        uint64_t comp = ten_pow - k;

        if (same_signature_uint64(k, comp)) {
            if (size == cap) {
                cap *= 2;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[size] = (seq_t)k;
            ++size;
        }

        ++k_current;
    }

    return cache[n - 1];
}
