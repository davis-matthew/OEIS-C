/*
 * A049950  a(n) = a(1) + a(2) + ... + a(n-1) + a(m) for n >= 4, where m = n -
 *          1 - 2^p and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), with a(1) = 1, a(2) = 2, and a(3) = 1.
 * https://oeis.org/A049950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2, a(3)=1; For n≥4: a(n) = sum_{k=1}^{n-1} a(k) +
 *            a(n-1-2^p), where 2^p < n-1 ≤ 2^(p+1)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *a_arr = NULL;
static seq_t *S_arr = NULL;
static idx_t max_cached = 0;
static size_t capacity = 0;

static void ensure_capacity(idx_t n)
{
    if (a_arr == NULL) {
        /* initial allocation */
        capacity = 8;
        a_arr = (seq_t *)malloc(capacity * sizeof(seq_t));
        S_arr = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!a_arr || !S_arr) return; /* out‑of‑memory scenario */

        /* base values (index 0 unused) */
        a_arr[0] = 0;
        S_arr[0] = 0;
        a_arr[1] = 1;
        S_arr[1] = 1;
        a_arr[2] = 2;
        S_arr[2] = 1 + 2;          /* 3 */
        a_arr[3] = 1;
        S_arr[3] = 1 + 2 + 1;      /* 4 */
        max_cached = 3;
    }

    idx_t need = n + 1;  /* we use indices 0..n */
    if ((size_t)need > capacity) {
        size_t new_cap = need * 2;
        a_arr = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
        S_arr = (seq_t *)realloc(S_arr, new_cap * sizeof(seq_t));
        if (!a_arr || !S_arr) return;
        capacity = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* out of range, but safe */

    ensure_capacity(n);
    if (n <= max_cached)
        return a_arr[n];

    /* Compute terms up to n */
    for (idx_t k = max_cached + 1; k <= n; ++k) {
        idx_t km1 = k - 1;      /* n-1 */
        idx_t pow = 1;          /* 2^p, start with 1 */

        /* find largest power of two strictly less than km1 */
        while ((pow << 1) < km1)
            pow <<= 1;

        idx_t m = km1 - pow;
        seq_t term = S_arr[k - 1] + a_arr[m];
        a_arr[k] = term;
        S_arr[k] = S_arr[k - 1] + term;
    }
    max_cached = n;
    return a_arr[n];
}
