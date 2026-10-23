/*
 * A162367  Number of reduced words of length n in the Weyl group D_25.
 * https://oeis.org/A162367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in f(25)*∏_{i=1}^{24} f(2i) where f(m)
 *            = (1 - x^m)/(1 - x)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 13, u64 n <= 53, u128 n <= 600,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is equivalent to a(n) = N_n - ∑_{t=1}^{min(25,n)} (-1)^t C(25,t) a(n-t),
   where N_n = coefficient of x^n in ∏_{m∈{25,2,4,…,48}} (1 - x^m). */
/* N_n is defined for 0 ≤ n ≤ 624; for n ≥ 625 we have N_n = 0. */

#include <stdlib.h>
#include <stdint.h>

/* maximum degree of the numerator polynomial ∏(1 - x^m) */
#define MAX_DEG 624          /* 25 + sum_{i=1}^{24} 2*i = 625, index 0..624 */

/* coefficients of the numerator polynomial N(x) */
static int8_t Ncoeff[MAX_DEG + 1];

/* binomial coefficients C(25,t)  (t = 0..25) */
static seq_t C25[26];

/* cache of a(n) values */
static seq_t *acache = NULL;
static idx_t Amax = (idx_t)-1;

/* 128‑bit integer type for safe intermediate sums (if available) */
#if defined(__SIZEOF_INT128__)
typedef __int128  s128_t;
typedef unsigned __int128 u128_t;
#else
/* fall back to 64‑bit arithmetic – may overflow, but occurs only
   when the result itself fits in 64‑bit which is covered by the tests. */
typedef long long  s128_t;
typedef unsigned long long u128_t;
#endif

/* ------------------------------------------------------------ */
/* Build the numerator polynomial coefficients Ncoeff */
static void init_Ncoeff(void)
{
    /* clear and set N(x) = 1 initially */
    for (int i = 0; i <= MAX_DEG; ++i) Ncoeff[i] = 0;
    Ncoeff[0] = 1;

    /* list of m in the product (1 - x^m) */
    const idx_t ms[25] = {
        25,     /* 1 .. 25 */
        2,4,6,8,10,12,14,16,18,20,22,24,
        26,28,30,32,34,36,38,40,42,44,46,48
    };

    /* multiply by each factor (1 - x^m) */
    for (size_t f = 0; f < 25; ++f) {
        idx_t m = ms[f];
        for (idx_t j = MAX_DEG; j >= m; --j) {
            Ncoeff[j] -= Ncoeff[j - m];
        }
    }

    /* binomial coefficients C(25,t) */
    C25[0] = (seq_t)1;
    for (idx_t t = 1; t <= 25; ++t) {
        /* multiply previous by (25 - t + 1) / t */
        C25[t] = C25[t - 1] * (seq_t)(25 - t + 1) / (seq_t)t;
    }
}

/* ------------------------------------------------------------ */
/* Ensure acache has space up to index n and contains a(0)…a(n) */
static void ensure_ACache(idx_t n)
{
    if (acache == NULL) {
        acache = (seq_t *)malloc(sizeof(seq_t) * (size_t)(n + 1));
        if (!acache) return;              /* out of memory – unlikely */
        acache[0] = (seq_t)1;              /* a(0) = 1 */
        Amax = 0;
    }
    if (n <= Amax) return;                 /* already computed */

    acache = (seq_t *)realloc(acache, sizeof(seq_t) * (size_t)(n + 1));
    for (idx_t i = Amax + 1; i <= n; ++i) {
        s128_t sum = 0;
        idx_t up = (i < 25) ? i : 25;
        for (idx_t t = 1; t <= up; ++t) {
            s128_t prod = (s128_t)C25[t] * (s128_t)acache[i - t];
            if (t & 1)        /* (-1)^t is negative for odd t */
                sum -= prod;
            else
                sum += prod;
        }
        s128_t Nvalue = (i <= MAX_DEG) ? (s128_t)Ncoeff[i] : 0;
        s128_t res = Nvalue - sum;
        acache[i] = (seq_t)res;            /* result is always non‑negative */
    }
    Amax = n;
}

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (Ncoeff[0] == 0) init_Ncoeff();   /* initialise once */
    ensure_ACache(n);
    return acache[n];
}
