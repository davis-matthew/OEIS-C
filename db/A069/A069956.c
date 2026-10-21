/*
 * A069956  Expansion of
 *          (1+x^2)/((1-x)^2*(1-x^2)^2*(1-x^3)^2*(1-x^8)*(1-x^9)*(1-x^10)).
 * https://oeis.org/A069956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 24, u32 n <= 166, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A069956
    Formula: expansion of (1+x^2)/((1-x)^2*(1-x^2)^2*(1-x^3)^2*(1-x^8)*(1-x^9)*(1-x^10))
    a(n) satisfies the linear recurrence
        a(n) = (n==0 || n==2 ? 1 : 0) 
               - sum_{k=1}^{39} d[k] * a(n-k)
    where d[k] are the coefficients of the denominator polynomial
    (1-x)^2*(1-x^2)^2*(1-x^3)^2*(1-x^8)*(1-x^9)*(1-x^10).
    The implementation computes the denominator once, then evaluates
    the recurrence iteratively, memoising results for arbitrary order
    calls.  All operations use only integer arithmetic as required.  */

#include <stdlib.h>
#include <string.h>

static seq_t *seq      = NULL;      /* memoised values */
static size_t seq_len  = 0;         /* number of values stored */
static size_t seq_cap  = 0;         /* allocated capacity */

static int den[40] = {0};           /* denominator coefficients   */
static int den_deg = 0;             /* highest degree of denom    */
static int init_done = 0;           /* flag whether denom is ready */

static inline seq_t mul_small(seq_t x, unsigned int m)
/* multiply x by small positive integer m by repeated addition
   (avoids overflow and does not depend on seq_t size) */
{
    seq_t r = 0;
    for (unsigned int i = 0; i < m; ++i)
        r += x;
    return r;
}

static void compute_denominator(void)
/* compute the coefficients of the denominator polynomial */
{
    int poly[40] = {0};
    int cur_deg = 0;
    poly[0] = 1;  /* start with 1 */

    /* (1 - x)^2 */
    {
        int f[3] = {1, -2, 1};
        int new_poly[40] = {0};
        for (int i = 0; i <= cur_deg; ++i)
            for (int j = 0; j <= 2; ++j)
                new_poly[i + j] += poly[i] * f[j];
        memcpy(poly, new_poly, sizeof poly);
        cur_deg += 2;
    }

    /* (1 - x^2)^2 */
    {
        int f[5] = {1, 0, -2, 0, 1};
        int new_poly[40] = {0};
        for (int i = 0; i <= cur_deg; ++i)
            for (int j = 0; j <= 4; ++j)
                new_poly[i + j] += poly[i] * f[j];
        memcpy(poly, new_poly, sizeof poly);
        cur_deg += 4;
    }

    /* (1 - x^3)^2 */
    {
        int f[7] = {1, 0, 0, -2, 0, 0, 1};
        int new_poly[40] = {0};
        for (int i = 0; i <= cur_deg; ++i)
            for (int j = 0; j <= 6; ++j)
                new_poly[i + j] += poly[i] * f[j];
        memcpy(poly, new_poly, sizeof poly);
        cur_deg += 6;
    }

    /* (1 - x^8) */
    {
        int f[9] = {1, 0, 0, 0, 0, 0, 0, 0, -1};
        int new_poly[40] = {0};
        for (int i = 0; i <= cur_deg; ++i)
            for (int j = 0; j <= 8; ++j)
                new_poly[i + j] += poly[i] * f[j];
        memcpy(poly, new_poly, sizeof poly);
        cur_deg += 8;
    }

    /* (1 - x^9) */
    {
        int f[10] = {1, 0, 0, 0, 0, 0, 0, 0, 0, -1};
        int new_poly[40] = {0};
        for (int i = 0; i <= cur_deg; ++i)
            for (int j = 0; j <= 9; ++j)
                new_poly[i + j] += poly[i] * f[j];
        memcpy(poly, new_poly, sizeof poly);
        cur_deg += 9;
    }

    /* (1 - x^10) */
    {
        int f[11] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1};
        int new_poly[40] = {0};
        for (int i = 0; i <= cur_deg; ++i)
            for (int j = 0; j <= 10; ++j)
                new_poly[i + j] += poly[i] * f[j];
        memcpy(poly, new_poly, sizeof poly);
        cur_deg += 10;
    }

    /* copy to global array */
    for (int i = 0; i <= cur_deg; ++i)
        den[i] = poly[i];
    den_deg = cur_deg;
}

static void ensure_capacity(size_t n)
/* ensure the memoisation array can hold indices up to n */
{
    if (seq_cap <= n) {
        size_t new_cap = n + 1;
        if (new_cap < 4) new_cap = 4;
        seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (seq != NULL) {
            memcpy(new_arr, seq, seq_len * sizeof(seq_t));
            free(seq);
        }
        for (size_t i = seq_len; i < new_cap; ++i)
            new_arr[i] = 0;
        seq = new_arr;
        seq_cap = new_cap;
    }
}

static void compute_up_to(size_t n)
/* extend memoised values up to index n */
{
    for (size_t i = seq_len; i <= n; ++i) {
        seq_t pos = 0, neg = 0;
        for (int k = 1; k <= den_deg && (size_t)k <= i; ++k) {
            int coeff = den[k];
            if (coeff > 0)
                pos += mul_small(seq[i - k], (unsigned int)coeff);
            else
                neg += mul_small(seq[i - k], (unsigned int)(-coeff));
        }
        seq_t rhs = (i == 0 || i == 2) ? 1 : 0;
        seq_t temp = rhs + neg;
        seq[i] = (pos <= temp) ? (temp - pos) : 0;   /* final result */
    }
    seq_len = n + 1;
}

seq_t a(idx_t n)
/* public function required by the test harness */
{
    if (!init_done) {
        compute_denominator();
        init_done = 1;
    }

    size_t idx = (size_t)n;
    ensure_capacity(idx);
    if (seq_len <= idx)
        compute_up_to(idx);
    return seq[idx];
}
