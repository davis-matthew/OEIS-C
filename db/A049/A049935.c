/*
 * A049935  a(n) = a(1) + a(2) + ... + a(n-1) + a(m) for n >= 4, where m = 2*n
 *          - 2 - 2^(p+1) and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), starting with a(1) = a(2) = a(3) = 1.
 * https://oeis.org/A049935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A049935
 * a(1) = a(2) = a(3) = 1
 * For n ≥ 4
 *   a(n) = (∑_{i=1}^{n-1} a(i)) + a(m)
 *   where m = 2·n − 2 − 2^{p+1}
 *   and p = ⌊log₂(n − 2)⌋   (unique integer such that 2^p < n−1 ≤ 2^{p+1}) */

/* Note:  the function keeps a static cache so that any call order works.
 *           The cache is expanded as needed. */

#include <stddef.h>
#include <stdlib.h>

/* integer log2 for unsigned 64‑bit values (v ≥ 1) */
static inline idx_t floor_log2_u64(idx_t v)
{
    idx_t r = 0;
    while (v >>= 1) ++r;
    return r;
}

seq_t a(idx_t n)
{
    /* static memoisation state */
    static seq_t *val = NULL;     /* values a(k)  */
    static seq_t *pref = NULL;   /* prefix sums S(k)=∑_{i=1}^k a(i) */
    static idx_t  computed = 0;   /* largest k already computed */
    static size_t cap = 0;        /* current allocation size (≥ computed+1) */

    /* ensure capacity for indices up to n (and at least 4 slots) */
    if (cap <= n || cap < 4) {
        size_t new_cap = (size_t)(n + 4);            /* give some buffer   */
        val = (seq_t *)realloc(val, new_cap * sizeof(seq_t));
        pref = (seq_t *)realloc(pref, new_cap * sizeof(seq_t));
        if (cap == 0) {
            pref[0] = 0;                             /* S(0)=0            */
        }
        cap = new_cap;
    }

    /* initialise the first three terms once */
    if (computed < 3) {
        val[1] = 1;  val[2] = 1;  val[3] = 1;
        pref[1] = val[1];
        pref[2] = pref[1] + val[2];
        pref[3] = pref[2] + val[3];
        computed = 3;
    }

    /* compute missing values up to n */
    for (idx_t k = computed + 1; k <= n; ++k) {
        idx_t p   = floor_log2_u64(k - 2U);
        idx_t pow = (idx_t)1U << (p + 1U);      /* 2^{p+1}           */
        idx_t m   = 2U * k - 2U - pow;          /* 1 ≤ m < k          */

        seq_t sum_before = pref[k - 1U];   /* S(k-1) */
        seq_t a_m = val[m];               /* a(m)   */
        seq_t a_k = sum_before + a_m;     /* a(k)   */

        val[k] = a_k;
        pref[k] = sum_before + a_k;       /* update prefix sum */
    }

    computed = (n > computed) ? n : computed;
    return val[n];
}
