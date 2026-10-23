/*
 * A025598  Number of n-move king paths on 8 X 8 board from given corner to
 *          adjacent corner.
 * https://oeis.org/A025598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 14, u64 n <= 26, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A025598: Number of n‑move king paths on an 8×8 board from a corner
   to an adjacent corner.  The generating function has a rational form with a
   denominator that is a product of seven factors of degree at most three.
   Expanding the denominator gives a linear recurrence of order 20:

       a(n) = Σ_{k=1}^{20} c_k · a(n‑k)     (n ≥ 20)

   with coefficients c_k = –Q_k where Q_k are the coefficients of the
   denominator polynomial Q(x) (Q_0 = 1).  The first 20 terms are:

       a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=0, a(6)=0,
       a(7)=127, a(8)=1568, a(9)=17221, a(10)=157710,
       a(11)=1360107, a(12)=11104632, a(13)=87999275,
       a(14)=681209386, a(15)=5192306420, a(16)=39117179936,
       a(17)=292250754994, a(18)=2169869100416,
       a(19)=16036792568508
*/

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
/* 1.  Recurrence coefficients   (built once)                    */
/* ------------------------------------------------------------ */
static int32_t Q_coef[21];   /* Q_coefs[0] = 1; Q_coef[k] = Q_k   */
static bool Q_built = false;

/* Multiply two polynomials stored in arrays of length ≤ 21
   The first polynomial has degree d1, second d2.  Result stored in dst. */
static void poly_mul(const int64_t *p1, int d1, const int64_t *p2, int d2, int64_t *dst)
{
    for (int i = 0; i <= d1 + d2; ++i) dst[i] = 0;
    for (int i = 0; i <= d1; ++i)
        for (int j = 0; j <= d2; ++j)
            dst[i + j] += p1[i] * p2[j];
}

static void build_Q(void)
{
    if (Q_built) return;

    /* Factorisation of the denominator:
       (3x-1)(x+1)(17x^3+6x^2-3x-1)
       (3x^3+9x^2+6x-1)(3x^3-9x^2-3x+1)
       (3x^3-3x-1)(x^3+3x^2-6x+1) */
    const int fac_deg[7] = {1, 1, 3, 3, 3, 3, 3};
    const int64_t fac_coef[7][4] = {
        { -1,  3, 0, 0 },          /* 3x-1          */
        {  1,  1, 0, 0 },          /* x+1           */
        { -1, -3, 6, 17 },         /* 17x^3+6x^2-3x-1*/
        { -1,  6, 9,  3 },         /* 3x^3+9x^2+6x-1*/
        {  1, -3, -9, 3 },         /* 3x^3-9x^2-3x+1*/
        { -1, -3, 0,  3 },         /* 3x^3-3x-1     */
        {  1, -6, 3,  1 }          /* x^3+3x^2-6x+1 */
    };

    int64_t cur[21] = {0};
    int carry_deg = 0;
    cur[0] = 1;   /* start with polynomial “1” */

    for (int f = 0; f < 7; ++f) {
        int d = fac_deg[f];
        int64_t next[21];
        poly_mul(cur, carry_deg, fac_coef[f], d, next);
        for (int i = 0; i <= carry_deg + d; ++i) cur[i] = next[i];
        carry_deg += d;
    }

    for (int i = 0; i <= carry_deg; ++i) Q_coef[i] = (int32_t)cur[i];
    Q_built = true;
}

/* ------------------------------------------------------------ */
/* 2.  First twenty terms of the sequence (offset 0)            */
/* ------------------------------------------------------------ */
static const seq_t init[20] = {
    0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL,  /* 0 … 6 */
    127ULL, 1568ULL, 17221ULL, 157710ULL,    /* 7 …10 */
    1360107ULL, 11104632ULL, 87999275ULL,   /* 11…13 */
    681209386ULL, 5192306420ULL, 39117179936ULL, /* 14…16 */
    292250754994ULL, 2169869100416ULL,       /* 17…18 */
    16036792568508ULL                        /* 19 */
};

/* ------------------------------------------------------------ */
/* 3.  Memoisation buffer                                   */
/* ------------------------------------------------------------ */
static seq_t *cache = NULL;
static idx_t max_cached = 0;

/* ------------------------------------------------------------ */
/* 4.  The sequence function                                 */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Build recurrence coefficients once */
    build_Q();

    /* Return initially known values */
    if (n < 20) return init[n];

    /* Init cache once */
    if (!cache) {
        cache = (seq_t *)calloc(20, sizeof(seq_t));
        for (idx_t i = 0; i < 20; ++i) cache[i] = init[i];
        max_cached = 19;
    }

    /* Grow cache to include n */
    if (n > max_cached) {
        size_t new_cap = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* Compute missing terms */
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            seq_t pos_sum = 0;
            seq_t neg_sum = 0;
            for (idx_t k = 1; k <= 20; ++k) {
                if (i < k) continue;
                seq_t term = (seq_t)( (idx_t)(Q_coef[k] < 0 ? -Q_coef[k] : Q_coef[k]) )
                            * cache[i - k];
                if (Q_coef[k] < 0)
                    pos_sum += term;
                else
                    neg_sum += term;
            }
            cache[i] = pos_sum - neg_sum;  /* pos_sum ≥ neg_sum by theory */
        }
        max_cached = n;
    }

    return cache[n];
}
