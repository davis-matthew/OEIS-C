/*
 * A162420  G.f.: A(x) = exp( Sum_{n>=1} sigma(n)*|A002129(n)|*x^n/n ).
 * https://oeis.org/A162420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = exp( Sum_{k>=1} sigma(k)*|A002129(k)|*x^k / k ) Computed
 *            by first computing |A002129(k)| via the recurrence |A002129(n)|
 *            = n*tri(n) - Σ_{k=1}^{n-1} |A002129(k)|*tri(n-k) where tri(m)=1
 *            if m is a triangular number else 0. Then a(0)=1 and for n>=1
 *            a(n)*n = Σ_{k=1}^{n} k*b(k)*a(n-k) with b(k)...
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   _floor_isqrt_u64, seq_alloc_array, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_alloc_array.h"   /* seq_alloc_array(size_t) -> seq_t* */
#include "../utils/seq_free.h"          /* seq_free(seq_t *) */
#include "../utils/_floor_isqrt_u64.h"  /* _floor_isqrt_u64(idx_t) */

static seq_t *sigma_arr  = NULL;      /* sigma(k) for 1..N       */
static seq_t *absT_arr  = NULL;      /* |A002129(k)| for 1..N   */
static seq_t *b_arr     = NULL;      /* b(k) = sigma(k)*|A002129(k)|   */
static seq_t *a_arr     = NULL;      /* a(n) for 0..N           */
static unsigned char *tri_flag = NULL;/* 1 if k triangular, else 0 */
static idx_t max_computed = 0;

/* helper to compute triangular test array up to n */
static void build_tri_flags(idx_t n)
{
    unsigned char *flags = (unsigned char *)malloc((n + 1) * sizeof(unsigned char));
    for (idx_t i = 0; i <= n; ++i) {
        unsigned long long v = 8ULL * i + 1;
        unsigned long long r = _floor_isqrt_u64(v);
        flags[i] = (r * r == v) ? 1 : 0;
    }
    free(tri_flag);
    tri_flag = flags;
}

/* build sigma array up to n */
static void build_sigma(idx_t n)
{
    seq_t *s = seq_alloc_array(n + 1);
    for (idx_t i = 0; i <= n; ++i) s[i] = (seq_t)0;
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i) {
            s[j] = s[j] + (seq_t)i;
        }
    }
    free(sigma_arr);
    sigma_arr = s;
}

/* compute |A002129(k)| up to n using recurrence with signed scratch */
static void build_absT(idx_t n)
{
    int64_t *tmp = (int64_t *)malloc((n + 1) * sizeof(int64_t));
    seq_t *arr  = seq_alloc_array(n + 1);
    tmp[0] = 0;
    arr[0] = (seq_t)0;
    for (idx_t m = 1; m <= n; ++m) {
        int64_t val = (int64_t)m * (int64_t)tri_flag[m];
        int64_t sum = 0;
        /* Σ_{k=1}^{m-1} tmp[k]*tri_flag[m-k] */
        for (idx_t k = 1; k < m; ++k) {
            if (tri_flag[m - k]) sum += tmp[k];
        }
        tmp[m] = val - sum;
        arr[m] = (tmp[m] >= 0) ? (seq_t)tmp[m] : (seq_t)(-tmp[m]);
    }
    free(absT_arr);
    absT_arr = arr;
    free(tmp);
}

/* build b(k) = sigma(k)*|A002129(k)| */
static void build_b(idx_t n)
{
    seq_t *b = seq_alloc_array(n + 1);
    for (idx_t i = 1; i <= n; ++i) {
        b[i] = sigma_arr[i] * absT_arr[i];
    }
    free(b_arr);
    b_arr = b;
}

/* build a(n) up to n using the exponential recurrence */
static void build_a(idx_t n)
{
    seq_t *a = seq_alloc_array(n + 1);
    a[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= i; ++k) {
            sum = sum + (b_arr[k] * a[i - k]);
        }
        a[i] = sum / (seq_t)i;
    }
    free(a_arr);
    a_arr = a;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n > max_computed) {
        /* extend all tables to n */
        build_tri_flags(n);
        build_sigma(n);
        build_absT(n);
        build_b(n);
        build_a(n);
        max_computed = n;
    }
    return a_arr[n];
}
