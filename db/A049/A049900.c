/*
 * A049900  a(n) = a(1) + a(2) + ... + a(n-1) - a(m) for n >= 4, where m = 2*n
 *          - 3 - 2^(p+1) and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), starting with a(1) = 1, a(2) = 2, and a(3) = 1.
 * https://oeis.org/A049900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2, a(3)=1. * For n≥4: a(n)=∑_{k=1}^{n-1}a(k) – a(m)
 *            where * m = 2*n – 3 – 2^(p+1) and * p = floor_log2(n-2)
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 35, u64 n <= 67, u128 n <= 132,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/floor_log2_uint64.h"

static idx_t cap        = 0;      /* highest n already computed */
static idx_t allocated  = 0;      /* current allocation size (index+1) */
static seq_t *cache     = NULL;   /* array of computed terms */
static seq_t sum        = 0;      /* sum of terms 1..cap */

seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n=0, return 0 */
        return (seq_t)0;

    if (n <= cap)          /* already computed */
        return cache[n];

    /* ensure the array is large enough */
    if (n + 1 > allocated) {
        size_t newsize = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!tmp)        /* allocation failure */
            abort();
        cache = tmp;
        allocated = newsize;
    }

    /* compute terms from cap+1 up to n */
    for (idx_t i = cap + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1) {
            val = (seq_t)1;
        } else if (i == 2) {
            val = (seq_t)2;
        } else if (i == 3) {
            val = (seq_t)1;
        } else {
            idx_t p = floor_log2_uint64(i - 2);        /* p such that 2^p < i-1 ≤ 2^(p+1) */
            seq_t pow2p1 = ((seq_t)1) << (p + 1);      /* 2^(p+1) */
            seq_t m = (seq_t)(2 * i - 3) - pow2p1;     /* subject term index */
            val = sum - cache[(idx_t)m];
        }
        cache[i] = val;
        sum += val;
    }

    cap = n;
    return cache[n];
}
