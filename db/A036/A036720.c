/*
 * A036720  G.f. satisfies A(x) = 1 + x*cycle_index(Klein_4_group, A(x)).
 * https://oeis.org/A036720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   A(x) = 1 + x * (A(x)^4 + 3 * A(x^2)^2) / 4 The recurrence for
 *            a(n) (n >= 1) is a(n) = (C(n-1) + 3 * B((n-1)/2)) / 4 if n is
 *            odd a(n) = C(n-1) / 4 if n is even where B(t) = sum_{i=0}^{t}
 *            a(i) * a(t-i) (coefficients of A(x)^2) C(t) = sum_{p=0}^{t} B(p)
 *            * B(t-p) (coefficients of A(x)^4)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 36, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
#include <type_traits>
using acc_t = std::conditional_t<
    std::is_same<seq_t, unsigned long long>::value ||
    std::is_same<seq_t, uint64_t>::value,
    unsigned __int128, seq_t>;
#else
typedef unsigned __int128 acc_t;
#endif

/* ensure that *arr has capacity at least needed, zero‑initialising
   any new elements. */
static void
ensure_cap_seq_t(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t new_cap = (*cap > 0) ? (*cap << 1) : 1;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp)
            exit(EXIT_FAILURE);
        for (size_t i = *cap; i < new_cap; ++i)
            tmp[i] = (seq_t)0;
        *arr   = tmp;
        *cap   = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* static state */
    static idx_t  max_n   = 0;            /* last computed index          */
    static seq_t *a_buf   = NULL;         /* array of a[0..max_n]         */
    static seq_t *b_buf   = NULL;         /* array of B[0..2*max_n]       */
    static size_t a_cap   = 0;
    static size_t b_cap   = 0;

    /* lazy initialization */
    if (a_buf == NULL) {
        a_cap = 1;
        a_buf = (seq_t *)malloc(a_cap * sizeof(seq_t));
        if (!a_buf) exit(EXIT_FAILURE);
        a_buf[0] = (seq_t)1;              /* a(0) = 1 */

        b_cap = 1;
        b_buf = (seq_t *)malloc(b_cap * sizeof(seq_t));
        if (!b_buf) exit(EXIT_FAILURE);
        b_buf[0] = (seq_t)1;              /* B(0) = a0*a0 = 1 */
        max_n = 0;
    }

    /* compute up to requested n (any order of calls) */
    while (max_n < n) {
        /* next index to compute */
        idx_t k = ++max_n;                /* compute a[k] */

        /* ensure capacities: a[0..k] and B[0..2*k] */
        ensure_cap_seq_t(&a_buf, &a_cap, k + 1);
        ensure_cap_seq_t(&b_buf, &b_cap, 2 * k + 1);

        idx_t m = k - 1;                  /* we need C(m) and maybe B(m/2) */

        /* compute C(m) = sum_{p=0}^{m} B[p] * B[m-p] */
        acc_t s1 = (acc_t)0;
        for (idx_t p = 0; p <= m; ++p) {
            acc_t prod = (acc_t)b_buf[p] * b_buf[m - p];
            s1 += prod;
        }

        /* compute optional 3 * B((m)/2) when m even (i.e. k odd) */
        acc_t s2 = (acc_t)0;
        if ((m & 1U) == 0) {
            idx_t half = m >> 1;
            s2 = (acc_t)3 * b_buf[half];
        }

        /* a[k] = (s1 + s2) / 4 */
        seq_t ak = (seq_t)((s1 + s2) / (acc_t)4);
        a_buf[k] = ak;

        /* update B[t] for t in [k .. 2*k] */
        /* for t = k .. 2*k-1: add 2 * a[k] * a[t-k] */
        for (idx_t t = k; t <= 2 * k - 1; ++t) {
            acc_t prod = (acc_t)ak * a_buf[t - k];
            prod += prod;                 /* double */
            b_buf[t] = (seq_t)(((acc_t)b_buf[t]) + prod);
        }
        /* t = 2*k: add a[k] * a[k] once */
        b_buf[2 * k] = (seq_t)(((acc_t)b_buf[2 * k]) + (acc_t)ak * ak);
    }

    return a_buf[n];
}
