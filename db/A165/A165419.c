/*
 * A165419  Each a(n) is chosen so that n = sum a(k), for all n >= 0, where k
 *          is over the distinct nonnegative values of the substrings in
 *          binary n.
 * https://oeis.org/A165419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = n - Σ_{k∈D(n), k≠n} a(k) where D(n) is the set of
 *            distinct non‑negative integers represented by substrings of the
 *            binary representation of n (including the number itself)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* forward declaration required for comparison function */
static int cmp_seq_t(const void *a, const void *b);

/* static cache state ---------------------------------------------- */
static seq_t *a_cache = NULL;          /* array of computed a(n) values    */
static idx_t cache_cap = 0;            /* allocated capacity in terms of n */
static idx_t cache_len = 0;            /* number of entries already computed */

/* quick size check of a 64‑bit integer to find its bit length   */
static int bitlen(idx_t x)
{
    int len = 0;
    do { len++; x >>= 1; } while (x);
    return len;
}

/* comparison function for qsort; works for any unsigned seq_t      */
static int cmp_seq_t(const void *p, const void *q)
{
    const seq_t *x = (const seq_t *)p;
    const seq_t *y = (const seq_t *)q;
    if (*x < *y) return -1;
    if (*x > *y) return  1;
    return 0;
}

/* Grow cache to at least “need” elements (need is a count, not index). */
static void ensure_cache_size(idx_t need)
{
    if (need <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    a_cache = tmp;
    cache_cap = new_cap;
}

/* main sequence function: ------------------------------------------------*/
seq_t a(idx_t n)
{
    /* allocate enough cache room */
    ensure_cache_size(n + 1);

    /* compute all missing entries up to n inclusive */
    for (idx_t m = cache_len; m <= n; ++m) {
        /* determine bit pattern of m */
        int len = bitlen(m);
        /* maximum substring count for 64 bits is 2080 */
        seq_t sub_vals[2100];
        idx_t sub_cnt = 0;

        /* bits stored from most significant to least significant */
        uint8_t bits[64];
        for (int i = 0; i < len; ++i)
            bits[i] = (m >> (len - 1 - i)) & 1u;

        /* enumerate all contiguous substrings */
        for (int i = 0; i < len; ++i) {
            seq_t val = 0;
            for (int j = i; j < len; ++j) {
                val = (val << 1) | bits[j];
                sub_vals[sub_cnt++] = val;
            }
        }

        /* sort and deduplicate the substring values */
        qsort(sub_vals, (size_t)sub_cnt, sizeof(seq_t), cmp_seq_t);
        idx_t uniq_cnt = 0;
        for (idx_t i = 0; i < sub_cnt; ++i) {
            if (uniq_cnt == 0 || sub_vals[i] != sub_vals[uniq_cnt - 1]) {
                sub_vals[uniq_cnt++] = sub_vals[i];
            }
        }

        /* sum contributions of smaller indices */
        seq_t sum = 0;
        for (idx_t i = 0; i < uniq_cnt; ++i) {
            idx_t k = (idx_t)sub_vals[i];
            if (k == m) continue;
            sum += a_cache[k];
        }

        /* compute a(m) */
        a_cache[m] = (seq_t)m - sum;
    }
    cache_len = n + 1;
    return a_cache[n];
}
