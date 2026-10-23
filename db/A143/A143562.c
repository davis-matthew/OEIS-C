/*
 * A143562  G.f. satisfies: A(x) = 1 + x*A(x)^3/A(-x)^2.
 * https://oeis.org/A143562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   A(x) = 1 + x*A(x)^3/A(-x)^2. The coefficients a(n) satisfy the
 *            recurrence a(0) = 1, a(n) = d_{n-1} – Σ_{j=1}^{n-1} a[n−j]·c_j
 *            (n≥1), where c_j = (−1)^j·s_j, s_j = Σ_{p+q=j} a_p·a_q, and
 *            d_{k} = Σ_{p+q+r=k} a_p·a_q·a_r = Σ_{p=0}^{k} a_p·s_{k−p}. This
 *            is computed in O(n²) time with memoised arrays
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Grow the dynamic array `arr` to at least `need` elements.
   `elem_size` is sizeof one element. */
static void grow_to(idx_t need, seq_t **arr, idx_t *cap, size_t elem_size)
{
    if (need < *cap)
        return;

    idx_t new_cap = *cap ? *cap << 1 : 16;
    while (need >= new_cap)
        new_cap <<= 1;

    *arr = (seq_t *)realloc(*arr, new_cap * elem_size);
    /* zero any new elements */
    size_t added = new_cap - *cap;
    if (added)
        memset((void *)((char *)*arr + (*cap) * elem_size), 0, added * elem_size);

    *cap = new_cap;
}

/* Return the n-th term of OEIS A143562. */
seq_t a(idx_t n)
{
    static seq_t *a_arr = NULL;   /* coefficients a[0..max] */
    static seq_t *s_arr = NULL;   /* convolution s[0..max]  */
    static idx_t cap_a = 0, cap_s = 0;
    static idx_t computed = 0;    /* highest index computed */

    grow_to(n + 1, &a_arr, &cap_a, sizeof(seq_t));
    grow_to(n + 1, &s_arr, &cap_s, sizeof(seq_t));

    /* initialise only once */
    if (computed == 0) {
        a_arr[0] = (seq_t)1;      /* a(0) = 1 */
        s_arr[0] = (seq_t)1;      /* s(0) = a0*a0 = 1 */
        computed = 0;             /* a(0) already known */
    }

    for (idx_t k = computed + 1; k <= n; ++k) {
        /* d_{k-1} = Σ_{p=0}^{k-1} a[p] * s[(k-1)-p] */
        seq_t d = 0;
        for (idx_t p = 0; p <= k - 1; ++p)
            d += a_arr[p] * s_arr[(k - 1) - p];

        /* sum_even and sum_odd for Σ_{j=1}^{k-1} a[k-j] · c_j ,
           with c_j = (−1)^j s_j.  Compute separately to avoid
           signed intermediate values. */
        seq_t sum_even = 0, sum_odd = 0;
        for (idx_t j = 1; j <= k - 1; ++j) {
            idx_t i = k - j;              /* i = n - j */
            seq_t prod = a_arr[i] * s_arr[j];
            if (j & 1)
                sum_odd += prod;
            else
                sum_even += prod;
        }

        seq_t a_k = d - sum_even + sum_odd;
        a_arr[k] = a_k;

        /* update s_k = Σ_{p=0}^k a[p] * a[k-p] */
        seq_t s_k = 0;
        for (idx_t p = 0; p <= k; ++p)
            s_k += a_arr[p] * a_arr[k - p];
        s_arr[k] = s_k;
    }

    computed = n;
    return a_arr[n];
}
