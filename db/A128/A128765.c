/*
 * A128765  Expansion of psi(q) * psi(q^10) / ( psi(q^2) * psi(q^5)) in powers
 *          of q where psi() is a Ramanujan theta function.
 * https://oeis.org/A128765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 25, i16 n <= 214, i32 n <= 484, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the n‑th coefficient of
 *    ∏_{k>0} (1 + x^k) · (1 + x^(10k))^2 / ( (1 + x^(2k))^2 · (1 + x^(5k)) )
 *
 * The product expansion satisfies the Euler transform
 *   a(n) = (1/n) · Σ_{k=1..n} s(k) · a(n−k)
 * where s(k) = Σ_{d|k} d · c_d and the period‑20 sequence
 *   c_d = 1, -2, 1, 0, 0, -2, 1, 0, 1, 0,
 *         1, 0, 1, -2, 0, 0, 1, -2, 1, 0  (period 20).
 *
 * The implementation below allocates two working arrays a[0..n] and s[1..n]
 * and evaluates the recurrence in O(n²).  It works for any seq_t/idx_t
 * type supplied by the build, including arbitrary‑precision integer
 * classes.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

static const int c_table[20] = {
    1, -2, 1, 0, 0, -2, 1, 0, 1, 0,
    1, 0, 1, -2, 0, 0, 1, -2, 1, 0
};

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;

    /* allocate working arrays a[0..n] and s[1..n] */
    idx_t size = n + (idx_t)1;
    seq_t *a_arr = NULL;
    seq_t *s_arr = NULL;

#ifdef __cplusplus
    try {
        a_arr = new (std::nothrow) seq_t[size];
        s_arr = new (std::nothrow) seq_t[size];
    } catch (...) { a_arr = NULL; s_arr = NULL; }
#else
    a_arr = (seq_t*)malloc((size_t)size * sizeof(seq_t));
    s_arr = (seq_t*)malloc((size_t)size * sizeof(seq_t));
#endif

    if (!a_arr || !s_arr) {
        /* allocation failure – return 0 (unlikely in typical use) */
#ifdef __cplusplus
        delete[] a_arr;
        delete[] s_arr;
#else
        free(a_arr);
        free(s_arr);
#endif
        return (seq_t)0;
    }

    /* initialize s[ ] */
    for (idx_t i = 1; i <= n; ++i) s_arr[i] = (seq_t)0;

    /* compute s(k) = Σ_{d|k} d·c_d for 1 ≤ k ≤ n */
    for (idx_t d = 1; d <= n; ++d) {
        int cd = c_table[(d - (idx_t)1) % (idx_t)20];
        if (cd == 0) continue;
        seq_t val = (seq_t)d * (seq_t)cd;
        for (idx_t m = d; m <= n; m += d)
            s_arr[m] += val;
    }

    /* recurrence a(0)=1, for cur≥1: a[cur]=(1/cur)·Σ_{k=1..cur} s[k]·a[cur−k] */
    a_arr[0] = (seq_t)1;
    for (idx_t cur = 1; cur <= n; ++cur) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= cur; ++k)
            sum += s_arr[k] * a_arr[cur - k];
        a_arr[cur] = sum / (seq_t)cur;   /* division is exact */
    }

    seq_t result = a_arr[n];

#ifdef __cplusplus
    delete[] a_arr;
    delete[] s_arr;
#else
    free(a_arr);
    free(s_arr);
#endif
    return result;
}
