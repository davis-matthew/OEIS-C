/*
 * A015486  a(0)=1, a(1)=2, a(n) = sum_{k=0}^{k=n-1} 2^k a(k).
 * https://oeis.org/A015486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(n) = Σ_{k=0}^{n-1} 2^k a(k) (n≥2)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_vals = NULL;     /* a(n) values */
static seq_t *s_vals = NULL;     /* s(n) = Σ_{k=0}^{n} 2^k a(k)  */
static seq_t *pow2_vals = NULL;  /* 2^i values */
static idx_t  a_len   = 0;        /* number of initialized a_vals elements */
static idx_t  pow2_len = 0;       /* number of initialized pow2_vals elements */

/* ensure pow2_vals contains 2^i for all i ≤ n */
static void ensure_pow2(idx_t n)
{
    if (pow2_len > n) return;

    seq_t *tmp = (seq_t *)realloc(pow2_vals, (n+1) * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    pow2_vals = tmp;

    if (pow2_len == 0) {
        pow2_vals[0] = (seq_t)1;
        pow2_len = 1;
    }
    for (idx_t i = pow2_len; i <= n; ++i)
        pow2_vals[i] = pow2_vals[i-1] * ((seq_t)2);
    pow2_len = n+1;
}

/* initialise and extend a_vals, s_vals up to index n */
static void ensure_sequence(idx_t n)
{
    /* initialise a[0] and s[0] */
    if (a_len == 0) {
        seq_t *tmp_a = (seq_t *)realloc(a_vals, sizeof(seq_t));
        seq_t *tmp_s = (seq_t *)realloc(s_vals, sizeof(seq_t));
        if (!tmp_a || !tmp_s) exit(EXIT_FAILURE);
        a_vals = tmp_a;  s_vals = tmp_s;
        a_vals[0] = (seq_t)1;
        s_vals[0] = (seq_t)1;
        a_len   = 1;
        pow2_len = 0;    /* will be expanded when needed */
    }

    /* handle a[1] specially */
    if (a_len == 1 && n >= 1) {
        ensure_pow2(1);
        seq_t *tmp_a = (seq_t *)realloc(a_vals, 2 * sizeof(seq_t));
        seq_t *tmp_s = (seq_t *)realloc(s_vals, 2 * sizeof(seq_t));
        if (!tmp_a || !tmp_s) exit(EXIT_FAILURE);
        a_vals = tmp_a;  s_vals = tmp_s;
        a_vals[1] = (seq_t)2;
        s_vals[1] = s_vals[0] + pow2_vals[1] * a_vals[1];
        a_len   = 2;
    }

    /* now compute for i ≥ 2 up to n */
    if (a_len <= n && a_len >= 2) {
        ensure_pow2(n);
        seq_t *tmp_a = (seq_t *)realloc(a_vals, (n+1) * sizeof(seq_t));
        seq_t *tmp_s = (seq_t *)realloc(s_vals, (n+1) * sizeof(seq_t));
        if (!tmp_a || !tmp_s) exit(EXIT_FAILURE);
        a_vals = tmp_a;  s_vals = tmp_s;
        for (idx_t i = a_len; i <= n; ++i) {
            a_vals[i] = s_vals[i-1];                    /* a(n) = S(n‑1) */
            seq_t temp = pow2_vals[i] + (seq_t)1;      /* 1 + 2^i */
            s_vals[i] = s_vals[i-1] * temp;            /* S(n) = S(n‑1) * (1+2^i) */
        }
        a_len = n + 1;
    }
}

seq_t a(idx_t n)
{
    ensure_sequence(n);
    return a_vals[n];
}
