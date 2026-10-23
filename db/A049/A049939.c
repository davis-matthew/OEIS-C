/*
 * A049939  a(n) = a(1) + a(2) + ... + a(n-1) + a(m) for n >= 4, where m = 2*n
 *          - 2 - 2^(p+1) and p is the unique integer such that 2^p < n-1 <=
 *          2^(p+1), with a(1) = a(2) = 1 and a(3) = 2.
 * https://oeis.org/A049939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(1) + a(2) + ... + a(n‑1) + a(m)  for n ≥ 4
 * where m = 2 n – 2 – 2^(p+1) and p is the unique integer such that
 * 2^p < n – 1 ≤ 2^(p+1).
 * Initial values: a(1)=1, a(2)=1, a(3)=2.
 *
 * The function is memoised and works for any call order.
 */

#include <stdint.h>
#include <stdlib.h>

/* Minimum number of stored terms (1‑based) */
static idx_t sz = 0;        /* highest index already computed */
static idx_t cap = 0;       /* capacity of the allocated arrays */

/* Dynamic containers for a(n) and the prefix sums pref[n] = a(1)+…+a(n) */
static seq_t *a_arr = NULL; /* a_arr[i] holds a(i) */
static seq_t *pref  = NULL; /* pref[i]  holds sum_{k=1..i} a(k) */

/* Compute ⌊log2(v)⌋ for 0 < v < 2^64; returns 0 if v==0 */
static inline idx_t floor_log2_u64(idx_t v)
{
    idx_t r = 0;
    while (v >>= 1)
        ++r;
    return r;
}

/* Ensure the arrays can hold indices up to `n` (inclusive). */
static void ensure_capacity(idx_t n)
{
    if (n <= cap)
        return;

    idx_t newcap = (cap == 0) ? 1 : cap << 1;
    while (newcap < n)
        newcap <<= 1;

#ifdef __cplusplus
    /* C++: use new[] so the seq_t objects are properly constructed. */
    seq_t *new_a    = new seq_t[newcap + 1];
    seq_t *new_pref = new seq_t[newcap + 1];
    /* copy already computed elements */
    for (idx_t i = 1; i <= sz; ++i) {
        new_a[i]    = a_arr[i];
        new_pref[i] = pref[i];
    }
    delete[] a_arr;
    delete[] pref;
    a_arr  = new_a;
    pref   = new_pref;
#else
    /* C: use realloc; seq_t is assumed trivially constructible. */
    a_arr  = (seq_t *)realloc(a_arr, (newcap + 1) * sizeof(seq_t));
    pref   = (seq_t *)realloc(pref, (newcap + 1) * sizeof(seq_t));
    /* Newly allocated elements are left uninitialised; they will be set
       before first use. */
#endif
    cap = newcap;
}

/* Public function implementing the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined, but safe fallback */

    ensure_capacity(n);

    if (sz >= n)
        return a_arr[n];

    /* Compute missing terms up to n. */
    for (idx_t i = sz + 1; i <= n; ++i) {
        if (i == 1) {
            a_arr[i] = 1;
            pref[i]  = 1;
            continue;
        }
        if (i == 2) {
            a_arr[i] = 1;
            pref[i]  = 2;
            continue;
        }
        if (i == 3) {
            a_arr[i] = 2;
            pref[i]  = 4;
            continue;
        }

        /* i ≥ 4: recurrence. */
        idx_t p = floor_log2_u64(i - 2);            /* 2^p < i-1 ≤ 2^(p+1) */
        idx_t m = 2 * i - 2 - ((idx_t)1 << (p + 1));/* m ≥ 1 */

        a_arr[i] = pref[i - 1] + a_arr[m];
        pref[i]  = pref[i - 1] + a_arr[i];
    }

    sz = n;
    return a_arr[n];
}
