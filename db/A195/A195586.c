/*
 * A195586  G.f.: exp( Sum_{n>=1} A163659(n^2)*x^n/n ), where x*exp(Sum_{n>=1}
 *          A163659(n)*x^n/n) = S(x) is the g.f. of Stern's diatomic series
 *          (A002487).
 * https://oeis.org/A195586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the functional equation A(x) = (1+x+x²)(1+x²+x⁴)
 *            · A(x²)² which yields the convolution recurrence a[n] =
 *            Σ_{k=0}^{6} f[k] · d[(n-k)/2] where f = {1,1,2,1,2,1,1} and d[m]
 *            = Σ_{i=0}^{m} a[i] · a[m-i] (convolution of a with itself)
 * Range:     u8 n <= 11, u16 n <= 27, u32 n <= 85, u64 n <= 279,
 *            u128 n <= 987, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* shared state */
static seq_t *a_arr = NULL;   /* holds a[0..max] */
static size_t a_cap = 0;      /* capacity of a_arr              */
static size_t a_len = 0;      /* number of a values computed    */

static seq_t *d_arr = NULL;   /* holds convolution d[0..max]   */
static size_t d_cap = 0;      /* capacity of d_arr              */

/* Extend the storage so that a[0..n] and d[0..max_needed] are available. */
static void ensure_upto(idx_t n)
{
    /* We need d[m] up to m = 2*n (max pair sum) after computing a[n]. */
    size_t needed_d_cap = (size_t)2 * (size_t)n + 1;
    if (needed_d_cap > d_cap) {
        size_t old_d_cap = d_cap;
        d_arr = (seq_t *)realloc(d_arr, needed_d_cap * sizeof(seq_t));
        if (!d_arr) exit(EXIT_FAILURE);
        if (old_d_cap == 0) {
            /* initialise all elements to 0 */
            for (size_t i = 0; i < needed_d_cap; ++i) d_arr[i] = (seq_t)0;
        } else {
            for (size_t i = old_d_cap; i < needed_d_cap; ++i) d_arr[i] = (seq_t)0;
        }
        d_cap = needed_d_cap;
    }

    size_t required_a_cap = (size_t)n + 1;
    if (a_cap < required_a_cap) {
        size_t new_a_cap = a_cap ? a_cap << 1 : 1;
        while (new_a_cap < required_a_cap) new_a_cap <<= 1;
        a_arr = (seq_t *)realloc(a_arr, new_a_cap * sizeof(seq_t));
        if (!a_arr) exit(EXIT_FAILURE);
        a_cap = new_a_cap;
    }

    /* initialise a(0) and d(0) once */
    if (a_len == 0) {
        a_arr[0] = (seq_t)1;          /* a(0) = 1 */
        d_arr[0] = a_arr[0] * a_arr[0]; /* d(0) = a0*a0 = 1 */
        a_len = 1;
    }

    static const seq_t f[7] = { (seq_t)1, (seq_t)1, (seq_t)2,
                                (seq_t)1, (seq_t)2, (seq_t)1, (seq_t)1 };

    /* compute a[cur] for cur = a_len .. n */
    for (idx_t cur = a_len; cur <= n; ++cur) {
        seq_t val = (seq_t)0;
        /* coefficient sum */
        for (int k = 0; k < 7; ++k) {
            if (cur < (idx_t)k) continue;
            idx_t diff = cur - (idx_t)k;
            if (diff & 1) continue; /* need even difference */
            size_t m = (size_t)(diff >> 1); /* m = (cur - k)/2 */
            val += f[k] * d_arr[m];
        }
        a_arr[(size_t)cur] = val;

        /* Update convolution array d for new a[cur] */
        for (size_t j = 0; j <= (size_t)cur; ++j) {
            size_t m = (size_t)cur + j;          /* index m = cur + j */
            seq_t prod = a_arr[(size_t)cur] * a_arr[j];
            if (j < cur) {
                d_arr[m] += prod;
                d_arr[m] += prod;                 /* (cur,j) and (j,cur) */
            } else {
                d_arr[m] += prod;                 /* j == cur, single appearance */
            }
        }
    }
    a_len = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    ensure_upto(n);
    return a_arr[(size_t)n];
}
