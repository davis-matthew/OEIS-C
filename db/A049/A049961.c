/*
 * A049961  a(n) = a(1) + a(2) + ... + a(n-1) + a(m) for n >= 3, where m =
 *          2^(p+1) + 2 - n and p is the smallest number such that 2^p < n - 1
 *          <= 2^(p+1), starting with a(1) = 1 and a(2) = 2.
 * https://oeis.org/A049961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(1) + a(2) + ... + a(n-1) + a(m) where m = 2^(p+1) + 2 -
 *            n, and p is the smallest such that 2^p < n-1 <= 2^(p+1). Initial
 *            values: a(1)=1, a(2)=2
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static inline unsigned floor_log2_uint64(idx_t v)
{
    /* Returns ⌊log₂(v)⌋ for v>0; loop is tiny since n≤10 000. */
    unsigned r = 0;
    idx_t tmp = 2;
    while (tmp <= v) {
        ++r;
        tmp <<= 1;
    }
    return r;
}

static seq_t *a_cache   = NULL; /* stored terms a(1)..a(cur_len) */
static seq_t *sum_cache = NULL; /* partial sums: sum_cache[i-1] = a(1)+...+a(i) */
static idx_t cur_len   = 0;     /* number of terms currently computed */
static idx_t cur_cap   = 0;     /* allocated capacity */

/* Ensure that terms 1…n are computed and stored. */
static void ensure_size(idx_t n)
{
    if (n <= cur_len)
        return;

    /* Grow arrays if needed. */
    if (cur_cap < n) {
        idx_t new_cap = cur_cap ? (cur_cap << 1) : 16;
        if (new_cap < n)
            new_cap = n;
        seq_t *tmp_a   = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        seq_t *tmp_sum = (seq_t*)realloc(sum_cache, new_cap * sizeof(seq_t));
        if (!tmp_a || !tmp_sum)
            abort();           /* memory exhausted */
        a_cache   = tmp_a;
        sum_cache = tmp_sum;
        cur_cap   = new_cap;
    }

    /* Compute missing terms. */
    for (idx_t i = cur_len + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1) {
            val = (seq_t)1;
        } else if (i == 2) {
            val = (seq_t)2;
        } else {
            idx_t p   = floor_log2_uint64(i - 2);
            idx_t two = ((idx_t)1) << (p + 1);
            idx_t m   = two + 2 - i;          /* 1 ≤ m ≤ i‑2, already computed */
            val = sum_cache[i - 2] + a_cache[m - 1];
        }
        a_cache[i - 1] = val;
        if (i == 1)
            sum_cache[0] = val;
        else
            sum_cache[i - 1] = sum_cache[i - 2] + val;
    }
    cur_len = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* defined for safety */
    ensure_size(n);
    return a_cache[n - 1];
}
