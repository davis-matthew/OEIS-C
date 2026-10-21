/*
 * A060627  1 + Sum_{n >= 1} Sum_{k = 0..n-1}
 *          (-1)^n*T(n,k)*y^(2*k)*x^(2*n)/(2*n)! = JacobiCN(x,y).
 * https://oeis.org/A060627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = the (n‑th) element of the triangle T(n,k) defined by the
 *            recurrence B_0(x) = 1, B_{m+1}(x) = B_m(x) + 2x (S_m(x) –
 *            B_m(x)) where S_m(x) = sum_{i+j+k=m} ( (2m)! / ((2i)! (2j)!
 *            (2k)!) ) B_i(x)B_j(x)B_k(x) and B_m(x) = sum_{k=0}^{m-1} T(m,k)
 *            x^k , (m≥1), B_0(x)=1. The sequence a(n) is obtaine...
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 29, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ----------  Includes  ---------- */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* seq_t and idx_t are supplied by the build environment. */

typedef struct {
    size_t len;   /* number of coefficients (degree + 1) */
    seq_t *coeff; /* coefficients array */
} poly_t;

/* ----------  Static storage  ---------- */
static idx_t B_len = 0;      /* number of rows already computed (max index+1) */
static poly_t *B = NULL;     /* array of polynomials B[0] … */

static idx_t fact_cap = 0;    /* factorial array currently usable */
static seq_t *fact = NULL;   /* factorials 0! … */

/* ----------  Helper functions ---------- */
static void ensure_factorial(idx_t n)
{
    if (fact_cap > n) return;
    idx_t old = fact_cap;
    fact_cap = n + 1;
    fact = (seq_t*)realloc(fact, fact_cap * sizeof(seq_t));
    if (old == 0) {  /* first time */
        fact[0] = (seq_t)1;
        old = 1;
    }
    for (idx_t i = old; i <= n; ++i) {
        fact[i] = fact[i-1] * (seq_t)i;
    }
}

static void ensure_B_capacity(idx_t new_cap)
{
    if (B_len >= new_cap) return;
    B_len = new_cap;
    B = (poly_t*)realloc(B, B_len * sizeof(poly_t));
}

static void poly_clear(poly_t *p)
{
    if (p->coeff) free(p->coeff);
    p->len = 0;
    p->coeff = NULL;
}

static void poly_init(poly_t *p, size_t len)
{
    poly_clear(p);
    p->len = len;
    if (len) {
        p->coeff = (seq_t*)calloc(len, sizeof(seq_t));
    } else {
        p->coeff = NULL;
    }
}

/* ----------  Compute rows up to 'r' (inclusive) ---------- */
static void compute_up_to(idx_t r)
{
    if (B_len > r) return;          /* already computed */

    /* Ensure factorials needed up to 2*r */
    ensure_factorial(2*r);

    /* initialise B[0] if necessary */
    if (B_len == 0) {
        ensure_B_capacity(1);
        poly_init(&B[0], 1);
        B[0].coeff[0] = (seq_t)1;    /* B_0 = 1 */
        B_len = 1;
    }

    /* loop adding rows until we have B[r] */
    for (idx_t m = B_len - 1; m < r; ++m) {
        /* S_m(x) : length m (degree <= m-1) */
        seq_t *S = (seq_t*)calloc(m, sizeof(seq_t));

        /* triple loop over i,j,k with i+j+k=m */
        for (idx_t i = 0; i <= m; ++i) {
            idx_t max_j = m - i;
            for (idx_t j = 0; j <= max_j; ++j) {
                idx_t k = m - i - j;

                /* multinomial coefficient */
                seq_t C = fact[2*m];
                C = C / fact[2*i];
                C = C / fact[2*j];
                C = C / fact[2*k];

                /* B_i * B_j  --> temp1 */
                size_t li = B[i].len;
                size_t lj = B[j].len;
                size_t len1 = li + lj - 1;
                seq_t *temp1 = (seq_t*)calloc(len1, sizeof(seq_t));

                for (size_t a = 0; a < li; ++a) {
                    seq_t aa = B[i].coeff[a];
                    for (size_t b = 0; b < lj; ++b) {
                        temp1[a+b] += aa * B[j].coeff[b];
                    }
                }

                /* temp1 * B_k --> temp2 */
                size_t lk = B[k].len;
                size_t len2 = len1 + lk - 1;
                seq_t *temp2 = (seq_t*)calloc(len2, sizeof(seq_t));

                for (size_t a = 0; a < len1; ++a) {
                    seq_t aa = temp1[a];
                    for (size_t b = 0; b < lk; ++b) {
                        temp2[a+b] += aa * B[k].coeff[b];
                    }
                }

                /* add to S */
                for (size_t d = 0; d < len2; ++d) {
                    S[d] += temp2[d] * C;
                }

                free(temp1);
                free(temp2);
            }
        }

        /* compute diff = S - B_m */
        seq_t *diff = (seq_t*)calloc(m, sizeof(seq_t));
        for (size_t d = 0; d < m; ++d) {
            diff[d] = S[d];
            if (d < B[m].len) diff[d] -= B[m].coeff[d];
        }

        /* new polynomial B_{m+1} : degree <= m, size m+1 */
        poly_t newB;
        poly_init(&newB, m+1);

        /* copy B_m */
        for (size_t d = 0; d < B[m].len; ++d) {
            newB.coeff[d] = B[m].coeff[d];
        }
        /* add 2x * diff  --> shift by 1 and scale by 2 */
        for (size_t d = 0; d < m; ++d) {
            newB.coeff[d+1] += diff[d] * (seq_t)2;
        }

        /* add newB to array */
        B_len = (m+2); /* because indices 0..m+1 now exist */
        B = (poly_t*)realloc(B, B_len * sizeof(poly_t));
        B[m+1] = newB;  /* ownership transferred */

        free(S);
        free(diff);
    }
}

/* ----------  Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;   /* undefined for n=0 */

    /* find row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) ++r;

    /* compute rows up to r if necessary */
    compute_up_to(r);

    /* index of element within the row */
    idx_t before = (r - 1) * r / 2;
    idx_t k = n - before - 1;   /* 0‑based column */

    return B[r].coeff[k];
}
