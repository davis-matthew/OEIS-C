/*
 * A127940  a(n) = denominator of b(n), where b(1)=1, b(n+1) = sum{k|n}
 *          1/b(k).
 * https://oeis.org/A127940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = denominator of b(n); b(1)=1, b(n+1)=∑_{k|n} 1/b(k)
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 15, u64 n <= 21, u128 n <= 26,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation keeps a small memoised table of b(n) in reduced form,
   using exact arithmetic provided by the build.                                       */

#include <stdlib.h>
#include "../utils/gcd.h"

typedef struct { seq_t num; seq_t den; } frac_t;

/* Memoised array of b values (1‑based).  The denominator is the requested term. */
static frac_t *b_arr   = NULL;   /* b[1..N] */
static size_t   b_cap  = 0;
static idx_t    max_n_computed = 0;

/* Smallest prime factor table for 1..N (uint64_t).  Only used for uint64_t build. */
static uint64_t *spf     = NULL;
static idx_t    spf_cap  = 0;

/* Build smallest‑prime‑factor array up to 'limit'.  Uses standard O(limit log log limit). */
static void build_spf(idx_t limit) {
    if (spf_cap > 0) free(spf);
    spf = (uint64_t *)malloc((limit + 1) * sizeof(uint64_t));
    for (idx_t i = 0; i <= limit; ++i) spf[i] = 0;
    if (limit >= 1) spf[1] = 1;
    for (idx_t i = 2; i <= limit; ++i) spf[i] = i;
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (spf[i] == i) {
            for (idx_t j = i * i; j <= limit; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }
    spf_cap = limit;
}

/* Ensure that b_arr has room for indices up to at least 'n' (inclusive). */
static void ensure_b_capacity(idx_t n) {
    if (b_cap > n) return;
    size_t new_cap = (b_cap == 0) ? 64 : b_cap * 2;
    while (new_cap <= (size_t)n) new_cap *= 2;
    b_arr = (frac_t *)realloc(b_arr, (new_cap + 1) * sizeof(frac_t));
    b_cap = new_cap;
}

/* Add fraction t_num/t_den to sum_num/sum_den, keeping it reduced. */
static void add_fraction(seq_t *sum_num, seq_t *sum_den, seq_t t_num, seq_t t_den) {
    seq_t g = gcd(*sum_den, t_den);
    seq_t nd = t_den / g;           /* multiplier for sum numerator          */
    seq_t ns = *sum_den / g;        /* multiplier for term numerator         */
    seq_t new_num = (*sum_num * nd) + (t_num * ns);
    seq_t new_den = (*sum_den * t_den) / g;
    seq_t g2 = gcd(new_num, new_den);
    *sum_num = new_num / g2;
    *sum_den = new_den / g2;
}

/* Compute all b values up to index 'n', storing them in b_arr. */
static void compute_up_to(idx_t n) {
    if (n <= max_n_computed) return;

    ensure_b_capacity(n);

    /* initialise b[1] if this is the first run */
    if (max_n_computed == 0) {
        b_arr[1].num = (seq_t)1;
        b_arr[1].den = (seq_t)1;
        max_n_computed = 1;
    }

    /* Ensure spf table large enough for factorisations of up to n-1. */
    if (spf_cap < (n > 0 ? n - 1 : 0)) build_spf((n > 0 ? n - 1 : 0));

    /* Temporary arrays for factorisation (small; 20 primes enough for n≤10⁴). */
    uint64_t primes[20];
    uint32_t exps[20];
    size_t   pcnt = 0;

    /* Temporary divisor buffer (divisors of numbers up to 10⁴ < 4096). */
    idx_t divs[4096];
    size_t dcnt;

    for (idx_t m = max_n_computed + 1; m <= n; ++m) {
        idx_t x = m - 1;                 /* need divisors of n-1                  */
        pcnt = 0;
        idx_t tmp = x;

        /* Factorise x using the spf table. */
        while (tmp > 1) {
            uint64_t p = spf[tmp];
            uint32_t cnt = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++cnt;
            }
            primes[pcnt] = p;
            exps[pcnt]  = cnt;
            ++pcnt;
        }

        /* Generate all divisors of x. */
        dcnt = 1;
        divs[0] = 1;
        for (size_t i = 0; i < pcnt; ++i) {
            idx_t  p   = primes[i];
            size_t exp = exps[i];
            size_t existing = dcnt;
            idx_t  mult = 1;
            for (size_t e = 1; e <= exp; ++e) {
                mult *= (idx_t)p;
                for (size_t j = 0; j < existing; ++j)
                    divs[dcnt++] = divs[j] * mult;
            }
        }

        /* Sum 1/b(d) for all divisors d of x. */
        seq_t sum_num = 0;
        seq_t sum_den = 1;
        for (size_t i = 0; i < dcnt; ++i) {
            idx_t d = divs[i];
            seq_t term_num = b_arr[d].den;   /* 1/b(d) = den/num           */
            seq_t term_den = b_arr[d].num;
            add_fraction(&sum_num, &sum_den, term_num, term_den);
        }

        /* Store the result in reduced form. */
        b_arr[m].num = sum_num;
        b_arr[m].den = sum_den;
    }

    max_n_computed = n;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    compute_up_to(n);
    return b_arr[n].den;
}
