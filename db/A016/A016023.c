/*
 * A016023  Inverse of 2014th cyclotomic polynomial.
 * https://oeis.org/A016023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   Let Q(x)=∏_{d|2014, d<2014} Φ_d(x) be the product of all proper
 *            cyclotomic polynomials of 2014. Then the inverse of Φ_2014(x)
 *            expands as 1/Φ_2014(x) = -Q(x)/(1-x^2014) so the coefficients
 *            are periodic with period 2014 and a(n) = -[Q]_{n mod 2014} . The
 *            implementation below builds Q once and retur...
 * Range:     i8 n <= 3000, i16 n <= 3000, i32 n <= 3000, i64 n <= 3000,
 *            i128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*  Poly structure for intermediate polynomials  */
typedef struct
{
    size_t deg;   /* degree of polynomial */
    seq_t *c;     /* coefficients, lowest order first */
} Poly;

/* Allocate a zeroed polynomial of given degree */
static Poly *poly_new_zero(size_t deg)
{
    Poly *p = (Poly *)malloc(sizeof(Poly));
    if (!p) return NULL;
    p->deg = deg;
    p->c   = (seq_t *)calloc(deg + 1, sizeof(seq_t));
    return p;
}

/* Allocate a constant polynomial 1 */
static Poly *poly_one(void)
{
    Poly *p = poly_new_zero(0);
    p->c[0] = 1;
    return p;
}

/* Allocate the polynomial (x^d - 1) */
static Poly *poly_xpow_minus_one(idx_t d)
{
    Poly *p = poly_new_zero(d);
    p->c[0] = -1;
    p->c[d] = 1;
    return p;
}

/* Free a polynomial structure */
static void poly_free(Poly *p)
{
    if (p)
    {
        free(p->c);
        free(p);
    }
}

/* Exact multiplication of two polynomials */
static Poly *poly_mul(const Poly *A, const Poly *B)
{
    size_t adeg = A->deg, bdeg = B->deg;
    size_t resdeg = adeg + bdeg;
    Poly *R = poly_new_zero(resdeg);
    for (size_t i = 0; i <= adeg; ++i)
        for (size_t j = 0; j <= bdeg; ++j)
            R->c[i + j] += A->c[i] * B->c[j];
    return R;
}

/* Exact division by a monic divisor (division is guaranteed to be exact) */
static Poly *poly_div(const Poly *NUM, const Poly *DEN)
{
    size_t ndeg = NUM->deg, ddeg = DEN->deg;
    size_t qdeg = ndeg - ddeg;
    seq_t *work = (seq_t *)malloc((ndeg + 1) * sizeof(seq_t));
    for (size_t i = 0; i <= ndeg; ++i) work[i] = NUM->c[i];
    seq_t *q = (seq_t *)calloc(qdeg + 1, sizeof(seq_t));

    for (long long i = (long long)ndeg; i >= (long long)ddeg; --i)
    {
        seq_t coeff = work[(size_t)i];
        if (coeff)
        {
            size_t qidx = (size_t)i - ddeg;
            q[qidx] = coeff;
            for (size_t j = 0; j <= ddeg; ++j)
                work[(size_t)i - ddeg + j] -= coeff * DEN->c[j];
        }
    }
    free(work);
    Poly *P = poly_new_zero(qdeg);
    P->c = q;
    return P;
}

/* Compute the cyclotomic polynomials Φ_d for all proper divisors of 2014 */
static void compute_all_phi(Poly *phi[], const idx_t *divs, size_t cnt)
{
    /* φ(1) = x - 1 */
    phi[0] = poly_new_zero(1);
    phi[0]->c[0] = -1;
    phi[0]->c[1] = 1;

    for (size_t i = 1; i < cnt; ++i)
    {
        idx_t d = divs[i];
        /* product of Φ_k for k | d, k < d */
        Poly *prod = poly_one();
        for (size_t j = 0; j < i; ++j)
            if (d % divs[j] == 0)
            {
                Poly *tmp = poly_mul(prod, phi[j]);
                poly_free(prod);
                prod = tmp;
            }
        Poly *num = poly_xpow_minus_one(d);
        phi[i] = poly_div(num, prod);
        poly_free(num);
        poly_free(prod);
    }
}

/* Sequence value a(n) */
seq_t a(idx_t n)
{
    static const idx_t P = 2014;
    static const idx_t divisors[7] = {1, 2, 19, 38, 53, 106, 1007};
    static bool init = false;
    static seq_t *coeff = NULL;

    if (!init)
    {
        /* Compute the product Q(x) = ∏_{d|2014, d<2014} Φ_d(x) */
        Poly *phi[7];
        compute_all_phi(phi, divisors, 7);

        Poly *Q = phi[0];
        for (size_t i = 1; i < 7; ++i)
        {
            Poly *tmp = poly_mul(Q, phi[i]);
            poly_free(Q);
            poly_free(phi[i]);   /* free after use */
            Q = tmp;
        }

        /* Store coefficients of Q into a static array of size P */
        coeff = (seq_t *)calloc(P, sizeof(seq_t));
        for (size_t i = 0; i <= Q->deg; ++i)
            coeff[i] = Q->c[i];
        poly_free(Q);
        init = true;
    }

    idx_t r = n % P;
    return -coeff[r];
}
