/*
 * A049903  a(n) = a(1) + a(2) + ... + a(n-1) - a(m) for n >= 4, where m = 2*n
 *          - 2 - 2^(p+1) and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), starting with a(1) = 1, a(2) = 2, and a(3) = 1.
 * https://oeis.org/A049903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(1) = 1, a(2) = 2, a(3) = 1
 *   for n ≥ 4:
 *     a(n) = (a(1)+a(2)+…+a(n-1)) − a(m)
 *     with m = 2*n − 2 − 2^(p+1)
 *     and p the unique integer such that 2^p < n−1 ≤ 2^(p+1)
 *
 * The function memoises results so that any order of calls is efficient.
 */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are supplied by the build environment. */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* out-of-range guard */

    static seq_t *cache      = NULL;         /* memoisation array */
    static size_t cap        = 0;            /* allocated capacity */
    static size_t computed   = 0;            /* largest n already computed */
    static seq_t prefix_sum   = 0;           /* sum of a(1)..a(computed) */

    /* Expand capacity if needed */
    if ((size_t)n > cap) {
        size_t newcap = cap ? cap * 2 : 128;
        while (newcap < (size_t)n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap   = newcap;
    }

    /* Compute values up to n if not already cached */
    if ((size_t)n > computed) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1) {
                val = (seq_t)1;
            } else if (i == 2) {
                val = (seq_t)2;
            } else if (i == 3) {
                val = (seq_t)1;
            } else {
                /* Find p: floor(log2(i-2))   (i ≥ 4) */
                idx_t tmp = i - 2;
                unsigned p = 0;
                while (tmp > 1) {          /* compute floor(log2(i-2)) */
                    tmp >>= 1;
                    ++p;
                }
                /* power = 2^(p+1) */
                idx_t power = ((idx_t)1 << (p + 1));
                /* m = 2*i - 2 - power   (1 ≤ m < i) */
                idx_t m = 2 * i - 2 - power;
                seq_t a_m = cache[m - 1];    /* m < i, already computed */
                val = prefix_sum - a_m;      /* prefix_sum holds sum(1..i-1) */
            }
            cache[i - 1] = val;
            prefix_sum += val;
        }
        computed = n;
    }

    return cache[n - 1];
}
