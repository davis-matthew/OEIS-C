/*
 * A012067  cos(arcsin(arcsin(x)))=1-1/2!*x^2-7/4!*x^4-169/6!*x^6-8751/8!*x^8...
 * https://oeis.org/A012067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = the integer coefficient of x^(2n)/(2n)! in
 *            cos(arcsin(arcsin(x))) = sqrt(1 - (arcsin(x))^2) Recurrence (for
 *            n ≥ 1): a(n) = - ((2n)! * d(n-1) + Σ_{i=1}^{n-1} C(2n,2i) * a(i)
 *            * a(n-i)) / 2 where d(m) = Σ_{k=0}^{m} c(k) * c(m-k) and c(k) =
 *            (2k)! / (4^k (k!)^2 (2k+1)). The implementation compu...
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 6, i64 n <= 10, i128 n <= 13,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"

static seq_t *a_cache = NULL;          /* computed a values */
static size_t a_cache_size = 0;        /* allocated size of a_cache */
static idx_t  a_cache_n     = 0;       /* number of values computed */

/* factorials up to 2*max_n */
static seq_t *fac = NULL;              
static size_t fac_size = 0;

/* powers of 4 up to max_n */
static seq_t *pow4 = NULL;             
static size_t pow4_size = 0;

/* num[k] = (2k)! */
static seq_t *num = NULL;             
static size_t num_size = 0;

/* den[k] = 4^k (k!)^2 (2k+1) */
static seq_t *den = NULL;             
static size_t den_size = 0;

/* ------------------------------------------------------------------ */
/*  Gcd helper that works on seq_t (provided by utils/gcd.h).          */
/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/*  Helper: binomial coefficient C(n,k) (exact integer).             */
/*  Uses multiplicative formula with exact division at each step.     */
/* ------------------------------------------------------------------ */
static seq_t binom_exact(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        result = (result * (seq_t)(n - k + i)) / (seq_t)i;
    }
    return result;
}

/* ------------------------------------------------------------------ */
/*  Expand the term (2n)! * c_k * c_{n-1-k} using safe reduction.     */
/*  Uses gcd to keep intermediate values small.                        */
/* ------------------------------------------------------------------ */
static seq_t term_for_k(seq_t fac2n, idx_t k, idx_t cur,
                       seq_t *num_out, seq_t *den_out,
                       seq_t *num_arr, seq_t *den_arr)
{
    /* local copies */
    seq_t term = fac2n;
    seq_t a = num_arr[k];
    seq_t b = num_arr[cur - 1 - k];
    seq_t d1 = den_arr[k];
    seq_t d2 = den_arr[cur - 1 - k];

    /* multiply by a, reduce with d1 */
    seq_t g = gcd(a, d1);
    a /= g; d1 /= g;
    g = gcd(term, d1);
    term /= g; d1 /= g;
    term *= a;

    /* multiply by b, reduce with d2 */
    g = gcd(b, d2);
    b /= g; d2 /= g;
    g = gcd(term, d2);
    term /= g; d2 /= g;
    term *= b;

    /* divide by remaining denominators */
    g = gcd(term, d1);
    term /= g; d1 /= g;
    g = gcd(term, d2);
    term /= g; d2 /= g;

    /* d1 and d2 should now be 1 */
    return term;
}

/* ------------------------------------------------------------------ */
/*  Main function computing a(n).                                     */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* trivial case */
    if (n == 0) return (seq_t)1;

    /* cast n for array indexing */
    size_t N = (size_t)n;

    /* recompute tables if needed */
    if (N + 1 > a_cache_size) {
        /* free old arrays */
        free(a_cache);
        free(fac);
        free(pow4);
        free(num);
        free(den);

        /* allocate new ones */
        a_cache_size = N + 1;
        a_cache     = (seq_t*)malloc(a_cache_size * sizeof(seq_t));
        fac_size    = 2 * N + 1;
        fac          = (seq_t*)malloc(fac_size * sizeof(seq_t));
        pow4_size   = N + 1;
        pow4         = (seq_t*)malloc(pow4_size * sizeof(seq_t));
        num_size    = N + 1;
        num          = (seq_t*)malloc(num_size * sizeof(seq_t));
        den_size    = N + 1;
        den          = (seq_t*)malloc(den_size * sizeof(seq_t));
    }

    /* factorials */
    fac[0] = 1;
    for (size_t i = 1; i <= 2 * N; ++i)
        fac[i] = fac[i - 1] * (seq_t)i;

    /* powers of 4 */
    pow4[0] = 1;
    for (size_t i = 1; i <= N; ++i)
        pow4[i] = pow4[i - 1] * (seq_t)4;

    /* num and den */
    for (size_t k = 0; k <= (size_t)n; ++k) {
        num[k] = fac[2 * k];
        seq_t t = pow4[k];
        t *= fac[k];
        t *= fac[k];
        t *= (seq_t)(2 * k + 1);   /* 4^k * (k!)^2 * (2k+1) */
        den[k] = t;
    }

    /* compute a values up to N */
    a_cache[0] = (seq_t)1;
    for (size_t cur = 1; cur <= N; ++cur) {
        seq_t sumD = 0;

        /* compute sumD = Σ_{k=0}^{cur-1} (2cur)! * c_k * c_{cur-1-k} */
        seq_t fac2n = fac[2 * cur];
        for (size_t k = 0; k < cur; ++k) {
            sumD += term_for_k(fac2n, k, cur,
                               num, den,
                               num, den);
        }

        /* compute S = Σ_{i=1}^{cur-1} C(2cur, 2i) * a(i) * a(cur-i) */
        seq_t S = 0;
        seq_t bin = 1; /* C(2cur,0) */
        for (size_t i = 1; i < cur; ++i) {
            /* update bin to C(2cur, 2i) from C(2cur,2i-2) */
            seq_t numA = 2 * cur - 2 * i + 2;
            seq_t numB = 2 * cur - 2 * i + 1;
            seq_t denA = 2 * i;
            seq_t denB = 2 * i - 1;

            /* reduce numerator with denominators */
            seq_t g = gcd(numA, denA);
            numA /= g; denA /= g;
            g = gcd(numB, denB);
            numB /= g; denB /= g;

            /* reduce bin with remaining denominators */
            g = gcd(bin, denA);
            bin /= g; denA /= g;
            g = gcd(bin, denB);
            bin /= g; denB /= g;

            /* multiply */
            bin = bin * numA;
            bin = bin * numB;

            /* final reductions (denA, denB should be 1) */
            g = gcd(bin, denA);
            bin /= g; denA /= g;
            g = gcd(bin, denB);
            bin /= g; denB /= g;

            S += bin * a_cache[i] * a_cache[cur - i];
        }

        /* compute a[cur] */
        a_cache[cur] = -((sumD + S) / 2);
    }

    a_cache_n = N + 1;
    return a_cache[n];
}
