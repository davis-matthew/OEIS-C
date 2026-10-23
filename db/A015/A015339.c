/*
 * A015339  Inverse of 1330th cyclotomic polynomial.
 * https://oeis.org/A015339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of n-th term in the power series expansion of
 *            1 / Φ_1330(x), where Φ_1330 is the 1330‑th cyclotomic
 *            polynomial. The sequence is periodic with period length 1330
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdbool.h>  /* bool, true, false */

/* Maximum degree needed for any intermediate polynomial during construction. */
#define MAX_DEG 2000

/* ---------- polynomial data structure ----------------------------------- */
typedef struct {
    size_t deg;                 /* degree of the polynomial */
    seq_t coeff[MAX_DEG + 1];   /* coeff[i] = coefficient of x^i, 0 <= i <= deg */
} Poly;

/* initialise a polynomial to zero of a given degree */
static void poly_init(Poly *p, size_t degree) {
    p->deg = degree;
    for (size_t i = 0; i <= degree; ++i) p->coeff[i] = 0;
}

/* multiply two polynomials: res = a * b */
static void poly_mul(const Poly *a, const Poly *b, Poly *res) {
    size_t d = a->deg + b->deg;
    poly_init(res, d);
    for (size_t i = 0; i <= a->deg; ++i) {
        for (size_t j = 0; j <= b->deg; ++j) {
            res->coeff[i + j] += a->coeff[i] * b->coeff[j];
        }
    }
}

/* divide polynomial num by monic divisor den, exact division: res = num / den */
static void poly_div(const Poly *num, const Poly *den, Poly *res) {
    size_t ndeg = num->deg;
    size_t ddeg = den->deg;
    size_t rdeg = ndeg - ddeg;          /* the result degree */
    poly_init(res, rdeg);

    seq_t tmp[MAX_DEG + 1];
    for (size_t i = 0; i <= ndeg; ++i) tmp[i] = num->coeff[i];

    for (int k = (int)ndeg; k >= (int)ddeg; --k) {
        seq_t coef = tmp[k];
        size_t pos = (size_t)(k - ddeg);
        res->coeff[pos] = coef;
        if (coef) {
            for (size_t j = 0; j <= ddeg; ++j) {
                tmp[pos + j] -= coef * den->coeff[j];
            }
        }
    }
}

/* build polynomial x^d - 1 */
static void poly_xd_minus1(size_t d, Poly *p) {
    poly_init(p, d);
    p->coeff[0] = -1;
    p->coeff[d] = 1;
}

/* ---------- divisor handling -------------------------------------------- */
static const idx_t N = 1330;                /* order of the cyclotomic polynomial */

/* generate all divisors of n in ascending order */
static void gen_divisors(idx_t n, idx_t divs[20], size_t *cnt) {
    size_t d = 0;
    for (size_t i = 1; i * i <= (size_t)n; ++i) {
        if (n % i == 0) {
            divs[d++] = (idx_t)i;
            if (i * i != (size_t)n)
                divs[d++] = (idx_t)(n / i);
        }
    }
    /* simple bubble sort – the number of divisors is small */
    for (size_t i = 0; i < d; ++i)
        for (size_t j = i + 1; j < d; ++j)
            if (divs[i] > divs[j]) {
                idx_t t = divs[i];
                divs[i] = divs[j];
                divs[j] = t;
            }
    *cnt = d;
}

/* compute Φ_d(x) for every divisor d of N and store them in poly_by_div */
static void compute_phi_polys(Poly poly_by_div[20], const idx_t divs[20], size_t cnt) {
    for (size_t idx = 0; idx < cnt; ++idx) {
        idx_t d = divs[idx];
        if (d == 1) {
            /* Φ₁(x) = x - 1 */
            poly_init(&poly_by_div[0], 1);
            poly_by_div[0].coeff[0] = -1;
            poly_by_div[0].coeff[1] = 1;
        } else {
            /* product of Φ_k(x) for all k | d, k < d */
            Poly prod;                     /* temporary product */
            poly_init(&prod, 0);           /* start with 1 */
            prod.coeff[0] = 1;
            for (size_t j = 0; j < idx; ++j) {   /* indices of smaller divisors */
                if (d % divs[j] == 0) {
                    Poly tmp;
                    poly_mul(&prod, &poly_by_div[j], &tmp);
                    prod = tmp;            /* copy assignment */
                }
            }
            /* numerator: x^d - 1 */
            Poly num;
            poly_xd_minus1((size_t)d, &num);
            /* divide to obtain Φ_d(x) */
            Poly res;
            poly_div(&num, &prod, &res);
            poly_by_div[idx] = res;        /* store result */
        }
    }
}

/* ---------- sequence period computation ---------------------------------- */

/* static storage for one period of length 1330 */
static seq_t period[1330];
static bool period_computed = false;

static void compute_period(void) {
    if (period_computed) return;

    idx_t divs[20];
    size_t cnt;
    gen_divisors(N, divs, &cnt);

    /* find index of divisor equal to N */
    size_t idx_N = 0;
    for (size_t i = 0; i < cnt; ++i)
        if (divs[i] == N) { idx_N = i; break; }

    Poly all_phi[20];
    compute_phi_polys(all_phi, divs, cnt);

    Poly phi = all_phi[idx_N];
    size_t deg = phi.deg;          /* φ(1330) = 432 */

    /* recurrence a(k) = -∑_{i=1..min(k,deg)} φ.coeff[i] * a(k-i) */
    seq_t a[1330];
    a[0] = 1;
    for (size_t k = 1; k < 1330; ++k) {
        seq_t s = 0;
        size_t upper = (k < deg) ? k : deg;
        for (size_t i = 1; i <= upper; ++i)
            s += phi.coeff[i] * a[k - i];
        a[k] = -s;
    }

    for (size_t i = 0; i < 1330; ++i)
        period[i] = a[i];

    period_computed = true;
}

/* ---------- public function --------------------------------------------- */

seq_t a(idx_t n) {
    compute_period();                        /* ensure period[ ] is ready */
    return period[(size_t)(n % 1330ULL)];
}
