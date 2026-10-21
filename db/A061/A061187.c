/*
 * A061187  Staircase of coefficients of polynomials used for column g.f.s of
 *          triangle A060924.
 * https://oeis.org/A061187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabf
 * Formula:   not stated
 * Range:     i8 n <= 10, i16 n <= 37, i32 n <= 42, i64 n <= 42, i128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Compute OEIS A061187: staircase of coefficients of polynomials 
 * pLo(n+1,x) where
 *   pLo(n+1,x) = (((1+x)+(3-2x)√x)^(n+1) - ((1+x)-(3-2x)√x)^(n+1))/(2√x).
 *
 * The sequence a(n) is obtained by listing the coefficients of all
 * these polynomials in order: pLo(1,x), pLo(2,x), pLo(3,x), … .
 *
 * The polynomials satisfy the linear recurrence
 *
 *     F_0(x) = 3 - 2x
 *     F_1(x) = 6 + 2x - 4x^2
 *     F_n(x) = 2(1+x)F_{n-1}(x) - (1 - 7x + 13x^2 - 4x^3)F_{n-2}(x)  for n ≥ 2
 *
 *  a(n) is the coefficient of x^m in the polynomial F_k(x) where the
 *  flattened index n falls into the block of coefficients of F_k.
 *
 * No floating point or math library functions are used.  All arithmetic
 * is performed with the generic integer type seq_t, whose range is
 * determined by the build.
 */

#include <stdlib.h>
#include <stddef.h>

/* Representation of one polynomial: an array of coefficients. */
typedef struct {
    idx_t          len;    /* number of coefficients (degree + 1) */
    seq_t *        coeff;  /* coeff[i] is the coefficient of x^i */
} poly_t;

/* Dynamic arrays of polynomials and their cumulative sizes. */
static poly_t  *polys     = NULL;
static size_t   polys_cap = 0;     /* allocated capacity for 'polys' */
static size_t   polys_used = 0;    /* number of polynomials stored */

static idx_t  *cumul     = NULL;   /* cumulative term counts */
static size_t  cumul_cap = 0;
static size_t  cumul_used = 0;     /* number of cumulative entries stored */

/* Ensure that the 'polys' array can hold at least 'need' elements. */
static void polys_ensure_capacity(size_t need)
{
    if (polys_cap < need) {
        size_t new_cap = polys_cap ? polys_cap * 2 : 4;
        if (new_cap < need) new_cap = need;
        polys = (poly_t*)realloc(polys, new_cap * sizeof(poly_t));
        polys_cap = new_cap;
    }
}

/* Ensure that the 'cumul' array can hold at least 'need' entries. */
static void cumul_ensure_capacity(size_t need)
{
    if (cumul_cap < need) {
        size_t new_cap = cumul_cap ? cumul_cap * 2 : 4;
        if (new_cap < need) new_cap = need;
        cumul = (idx_t*)realloc(cumul, new_cap * sizeof(idx_t));
        cumul_cap = new_cap;
    }
}

/* Compute all polynomials up to index 'target' (inclusive). */
static void ensure_polys_upto(idx_t target)
{
    /* --- base polynomials --- */
    if (polys_used == 0) {
        polys_ensure_capacity(2);
        polys[0].len   = 2;
        polys[0].coeff = (seq_t*)malloc(sizeof(seq_t)*2);
        polys[0].coeff[0] = (seq_t)3;
        polys[0].coeff[1] = (seq_t)(-2);
        polys_used = 1;
    }
    if (polys_used == 1) {
        polys_ensure_capacity(2);
        polys[1].len   = 3;
        polys[1].coeff = (seq_t*)malloc(sizeof(seq_t)*3);
        polys[1].coeff[0] = (seq_t)6;
        polys[1].coeff[1] = (seq_t)2;
        polys[1].coeff[2] = (seq_t)(-4);
        polys_used = 2;
    }

    /* --- recurrence for higher indices --- */
    while (polys_used <= target) {
        poly_t const *prev1 = &polys[polys_used-1];
        poly_t const *prev2 = &polys[polys_used-2];

        /* (1+x) * prev1, then multiplied by 2 */
        idx_t t1_len = prev1->len + 1;
        seq_t *temp1 = (seq_t*)malloc(sizeof(seq_t)*t1_len);
        temp1[0] = 2 * prev1->coeff[0];
        for (idx_t i = 1; i < prev1->len; ++i)
            temp1[i] = 2 * (prev1->coeff[i] + prev1->coeff[i-1]);
        temp1[t1_len-1] = 2 * prev1->coeff[prev1->len-1];

        /* (1-7x+13x^2-4x^3) * prev2 */
        idx_t t2_len = prev2->len + 3;          /* deg(prev2)+3 +1 */
        seq_t *temp2 = (seq_t*)calloc(t2_len, sizeof(seq_t));   /* zeroed */
        const seq_t Bcoeff[4] = { (seq_t)1, (seq_t)(-7), (seq_t)13, (seq_t)(-4) };
        for (idx_t i = 0; i < prev2->len; ++i) {
            seq_t a = prev2->coeff[i];
            temp2[i]   += a * Bcoeff[0];
            temp2[i+1] += a * Bcoeff[1];
            temp2[i+2] += a * Bcoeff[2];
            temp2[i+3] += a * Bcoeff[3];
        }

        /* new polynomial: temp1 - temp2 */
        idx_t new_len = t1_len > t2_len ? t1_len : t2_len;
        seq_t *new_coeff = (seq_t*)malloc(sizeof(seq_t)*new_len);
        for (idx_t i = 0; i < new_len; ++i) {
            seq_t v = 0;
            if (i < t1_len) v += temp1[i];
            if (i < t2_len) v -= temp2[i];
            new_coeff[i] = v;
        }

        free(temp1);
        free(temp2);

        polys_ensure_capacity(polys_used + 1);
        polys[polys_used].len = new_len;
        polys[polys_used].coeff = new_coeff;
        polys_used++;
    }
}

/* Return the nth term of the sequence.  Indexing starts at 0. */
seq_t a(idx_t n)
{
    idx_t poly_index = 0;
    idx_t cumulative  = 0;

    ensure_polys_upto(0);                /* makes sure base polynomials exist */

    for (;;) {
        if (poly_index >= polys_used)           /* request next polynomial */
            ensure_polys_upto(poly_index);

        idx_t len = polys[poly_index].len;
        if (n < cumulative + len)              /* term lies in this block */
            return polys[poly_index].coeff[n - cumulative];

        cumulative += len;
        poly_index++;
    }
}
