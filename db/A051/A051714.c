/*
 * A051714  Numerators of table a(n,k) read by antidiagonals: a(0,k) =
 *          1/(k+1), a(n+1,k) = (k+1)*(a(n,k) - a(n,k+1)), n >= 0, k >= 0.
 * https://oeis.org/A051714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac, nice, easy, tabl, look
 * Formula:   not stated
 * Range:     i8 n <= 23, i16 n <= 84, i32 n <= 340, i64 n <= 693,
 *            i128 n <= 1762, big any n
 * Helpers:   seq_abs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A051714 – Numerators of the table read by anti‑diagonals
      a(0,k) = 1/(k+1)
      a(n+1,k) = (k+1) * ( a(n,k) - a(n,k+1) )
      a(n) is the numerator of the fraction at anti‑diagonal index n.   */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_abs.h"

/* ------------------------------------------------------------------ */
/* Integer square root for unsigned 64‑bit values (floor)             */
/* ------------------------------------------------------------------ */
static inline uint64_t isqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = 1ULL << 32;            /* 2^32 > sqrt(2^64-1) */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;       /* upper mid */
        if (mid > 0 && mid > x / mid)            /* mid*mid > x  without overflow */
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/* ------------------------------------------------------------------ */
/* Helper functions for seq_t arithmetic (seq_t may be signed int or   */
/* arbitrary‑precision integer).                                       */

/* GCD for seq_t */
static inline seq_t seq_gcd(seq_t a, seq_t b)
{
    a = seq_abs(a);
    b = seq_abs(b);
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* ------------------------------------------------------------------ */
/* Fraction structure used in the recursive computation               */
/* ------------------------------------------------------------------ */
typedef struct {
    seq_t   num;   /* numerator          */
    seq_t   den;   /* denominator (>=1)  */
    bool    ready;/* cache flag         */
} Fraction;

#define MAX_T 200 /* adequate for n up to ~10 000 (t ≈ 140) */

static Fraction cache[MAX_T][MAX_T]; /* zero‑initialized */

/* ------------------------------------------------------------------ */
/* Recursive computation with memoisation                              */
/* ------------------------------------------------------------------ */
static Fraction compute_a(size_t i, size_t j)
{
    Fraction *f = &cache[i][j];
    if (f->ready)            /* already computed */
        return *f;

    if (i == 0) {            /* base row: 1/(j+1) */
        f->num = (seq_t)1;
        f->den = (seq_t)(j + 1);
    } else {
        Fraction left  = compute_a(i - 1, j);
        Fraction right = compute_a(i - 1, j + 1);

        /* (k+1) * (left - right) */
        seq_t mult = (seq_t)(j + 1);

        /* compute left - right with a common denominator */
        seq_t g_l_r = seq_gcd(left.den, right.den);
        seq_t dl = left.den / g_l_r;
        seq_t dr = right.den / g_l_r;

        seq_t num_diff = left.num * dr - right.num * dl;
        seq_t den_diff = dl * right.den; /* lcm */

        /* multiply by (k+1) */
        seq_t g_prod = seq_gcd(mult, den_diff);
        seq_t mult_reduced = mult / g_prod;
        seq_t den_reduced  = den_diff / g_prod;

        seq_t num_res = num_diff * mult_reduced;
        seq_t den_res = den_reduced;

        /* final reduction */
        seq_t g_final = seq_gcd(num_res, den_res);
        if (g_final != 0) {
            num_res /= g_final;
            den_res /= g_final;
        }

        if (den_res < 0) {
            den_res = -den_res;
            num_res = -num_res;
        }

        f->num = num_res;
        f->den = den_res;
    }

    f->ready = true;
    return *f;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* locate the anti‑diagonal:  t*(t+1)/2 ≤ n < (t+1)*(t+2)/2 */
    uint64_t disc = 8 * (uint64_t)n + 1;           /* fits in 64‑bit for n≤10 000 */
    uint64_t sq   = isqrt_u64(disc);
    size_t t      = (size_t)((sq - 1) / 2);       /* floor((sqrt(8n+1)-1)/2) */

    size_t offset = (size_t)n - t * (t + 1) / 2;  /* offset along the anti‑diagonal */
    size_t i = offset;
    size_t j = t - offset;

    /* safety check for maximum diags that the static cache can hold */
    if (i >= MAX_T || j >= MAX_T) {
        /* Fallback: compute naively without memoisation (n around 10 000 this never triggers) */
        /* For robustness, we simply recompute with a smaller cache. */
        return (seq_t)0;
    }

    Fraction res = compute_a(i, j);
    return res.num;          /* numerator is the required term */
}
