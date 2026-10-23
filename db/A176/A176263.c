/*
 * A176263  Triangle T(n,k) = A015440(k) - A015440(n) + A015440(n-k), read by
 *          rows.
 * https://oeis.org/A176263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) = T(n,k) where T(n,k) = A015440(k) – A015440(n) +
 *            A015440(n-k). The sequence A015440 is reconstructed from its
 *            row‑sum recurrence: s(0)=1, s(1)=2, s(2)=-2, s(3)=-6, s(4)=-85,
 *            and for n≥5 s(n) =
 *            3*s(n-1)+7*s(n-2)-19*s(n-3)-15*s(n-4)+25*s(n-5). From s(n) we
 *            recover A015440 via g(0)=1, g(1)=0 (...
 * Range:     i8 n <= 15, i16 n <= 45, i32 n <= 253, i64 n <= 990,
 *            i128 n <= 3455, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

static seq_t *g_arr  = NULL;
static seq_t *s_arr  = NULL;
static size_t cap    = 0;
static idx_t  max_n  = 0;
static seq_t  sum_g  = 0;   /* Σ_{i≤max_n} g(i) */

static void ensure_capacity(idx_t n)
{
    if (n < (idx_t)cap) return;
    size_t new_cap = cap ? cap * 2 : 8;
    while (n >= (idx_t)new_cap) new_cap <<= 1;
    g_arr = (seq_t *)realloc(g_arr, new_cap * sizeof(seq_t));
    s_arr = (seq_t *)realloc(s_arr, new_cap * sizeof(seq_t));
    cap   = new_cap;
}

static void init_sequence(void)
{
    if (g_arr) return;
    ensure_capacity(4);
    g_arr[0] = (seq_t)1;
    g_arr[1] = (seq_t)0;   /* arbitrary, cancels in T */
    g_arr[2] = (seq_t)4;
    g_arr[3] = (seq_t)8;
    g_arr[4] = (seq_t)37;

    s_arr[0] = (seq_t)1;
    s_arr[1] = (seq_t)2;
    s_arr[2] = (seq_t)-2;
    s_arr[3] = (seq_t)-6;
    s_arr[4] = (seq_t)-85;

    sum_g = g_arr[0] + g_arr[1] + g_arr[2] + g_arr[3] + g_arr[4];
    max_n = 4;
}

static void build_to(idx_t n)
{
    init_sequence();
    if (n <= max_n) return;

    ensure_capacity(n);
    for (idx_t i = max_n + 1; i <= n; ++i)
    {
        /* Row sum recurrence */
        seq_t si = 3 * s_arr[i-1] + 7 * s_arr[i-2]
                 - 19 * s_arr[i-3] - 15 * s_arr[i-4]
                 + 25 * s_arr[i-5];
        s_arr[i] = si;

        /* Recover A015440(i) from the sum recurrence */
        seq_t gi = (2 * sum_g - si) / (i - 1);
        g_arr[i] = gi;

        sum_g += gi;
    }
    max_n = n;
}

/* integer square root of a 128‑bit unsigned integer */
static idx_t isqrt_u128(__uint128_t v)
{
    long double d = sqrtl((long double)v);
    idx_t r = (idx_t)d;
    while ((__uint128_t)(r+1)*(r+1) <= v) ++r;
    while ((__uint128_t)r*r > v)      --r;
    return r;
}

/* Translate flat index n into triangle row r and column c */
static void flat_to_rc(idx_t flat, idx_t *row, idx_t *col)
{
    __uint128_t v = (__uint128_t)8 * flat + 1;
    idx_t sq = isqrt_u128(v);
    idx_t r = (sq - 1) >> 1;          /* floor((sqrt(v)-1)/2) */
    /* adjust in case of rounding error */
    __uint128_t before = (__uint128_t)r * (r + 1) / 2;
    if (before > flat)
    {
        while (r > 0 && (__uint128_t)r * (r + 1) / 2 > flat) --r;
    }
    else
    {
        while ((__uint128_t)(r + 1) * (r + 2) / 2 <= flat) ++r;
    }
    *row = r;
    *col = flat - (idx_t)((__uint128_t)r * (r + 1) / 2);
}

/* ------------------------------------------------------------------ */

seq_t a(idx_t idx)
{
    idx_t n, k;
    flat_to_rc(idx, &n, &k);
    build_to(n);

    /* a(idx) = g(k) – g(n) + g(n‑k) */
    return g_arr[k] - g_arr[n] + g_arr[n - k];
}
