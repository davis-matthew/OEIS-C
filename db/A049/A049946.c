/*
 * A049946  a(n) = a(1) + a(2) + ... + a(n-1) + a(m) for n >= 4, where m = n -
 *          1 - 2^p and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), starting with a(1) = a(2) = 1 and a(3) = 4.
 * https://oeis.org/A049946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(1)+a(2)+...+a(n-1)+a(m) with m = (n-1)-2^p, where p is
 *            floor(log2(n-2)). Initial values: a(1)=1, a(2)=1, a(3)=4
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Helper: floor(log2(v)) for non‑zero v */
static inline idx_t floor_log2_uint64(idx_t v) {
    return 63 - __builtin_clzll(v);   /* v>0, 63‑clzll gives floor log2 */
}

/* Static storage for memoisation */
static seq_t *seq     = NULL;   /* a(i)  – 1‑based, seq[0] unused */
static seq_t *pref    = NULL;   /* prefix sums, pref[0] = 0 */
static size_t  cap    = 0;      /* capacity of allocated arrays (>=1) */
static size_t  computed = 0;     /* highest index already computed */

/* Ensure capacity for at least n elements (1‑based indices) */
static void ensure_capacity(idx_t n) {
    if ((size_t)n <= cap) return;
    size_t new_cap = cap ? cap * 2 : 8;
    while ((size_t)n > new_cap) new_cap <<= 1;
    seq  = (seq_t *)realloc(seq,  (new_cap + 1) * sizeof(seq_t));
    pref = (seq_t *)realloc(pref, (new_cap + 1) * sizeof(seq_t));
    if (pref) pref[0] = (seq_t)0;  /* prefix sum before first term */
    cap = new_cap;
}

/* Sequence function */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;           /* undefined for n<1 */
    ensure_capacity(n);

    /* Compute any missing terms up to n */
    for (size_t i = computed + 1; i <= (size_t)n; ++i) {
        if (i == 1) {
            seq[i] = (seq_t)1;
        } else if (i == 2) {
            seq[i] = (seq_t)1;
        } else if (i == 3) {
            seq[i] = (seq_t)4;
        } else {
            /* For i >= 4 we compute a(i) = pref[i-1] + a(m) */
            idx_t p = floor_log2_uint64((idx_t)i - 2);   /* p = floor log2(i-2) */
            idx_t pow2 = (idx_t)1 << p;                  /* 2^p */
            idx_t m = (i - 1) - pow2;                    /* m is 1‑based */
            seq[i] = pref[i - 1] + seq[m];
        }
        pref[i] = pref[i - 1] + seq[i];
    }
    computed = (size_t)n;
    return seq[(size_t)n];
}
