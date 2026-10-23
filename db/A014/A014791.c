/*
 * A014791  Inverse of 782nd cyclotomic polynomial.
 * https://oeis.org/A014791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -∑_{k=1}^{φ(782)} Φ_782(k)·a(n‑k), a(0)=1. The sequence
 *            is periodic with period 782
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* auxiliary: divide polynomial A by monic polynomial B.
   A and B are given by ascending coefficient arrays.
   Returns the quotient Q (stored in the array Q) and
   the quotient size.  Division is exact. */
static size_t poly_div(const seq_t *A, size_t a_size,
                       const seq_t *B, size_t b_size,
                       seq_t *Q)
{
    /* maximum polynomial size needed (degree ≤ 782) */
    enum { MAXP = 783 };
    seq_t R[MAXP];
    for (size_t i = 0; i < a_size; ++i) R[i] = A[i];

    size_t q_size = a_size - b_size + 1;
    for (size_t i = 0; i < q_size; ++i) Q[i] = 0;

    int lead = (int)b_size - 1;
    for (int ii = (int)a_size - 1; ii >= lead; --ii) {
        seq_t coeff = R[ii];
        int qpos = ii - lead;
        Q[qpos] = coeff;
        for (size_t j = 0; j < b_size; ++j) {
            R[ii - lead + j] -= coeff * B[j];
        }
    }
    return q_size;
}

/* main routine that precomputes the periodic table once */
static void build_table(void)
{
    enum { PERIOD = 782, MAXP = 783 };
    static seq_t table[PERIOD];

    /* divisor list of 782 in increasing order */
    const int divs[8] = {1, 2, 17, 23, 34, 46, 391, 782};

    /* container for the cyclotomic polynomials Φ_d */
    struct Poly {
        size_t size;
        seq_t coeff[MAXP];
    };
    struct Poly polys[8];

    /* compute Φ_d for each divisor */
    for (int idx = 0; idx < 8; ++idx) {
        int d = divs[idx];
        struct Poly p;
        p.size = d + 1;
        for (size_t i = 0; i < p.size; ++i) p.coeff[i] = 0;
        p.coeff[0] = -1;          /* x^d – 1 */
        p.coeff[d] = 1;

        /* divide by all previously computed Φ_e where e | d */
        for (int j = 0; j < idx; ++j) {
            int e = divs[j];
            if (d % e == 0) {
                seq_t tmp[MAXP];
                size_t qsz = poly_div(p.coeff, p.size,
                                      polys[j].coeff, polys[j].size,
                                      tmp);
                p.size = qsz;
                for (size_t k = 0; k < qsz; ++k) p.coeff[k] = tmp[k];
            }
        }
        polys[idx] = p;
    }

    /* φ₇₈₂ has degree φ(782)=352 */
    const struct Poly *phi = &polys[7];
    size_t deg = phi->size - 1;

    /* compute the period of length 782 */
    seq_t a[PERIOD];
    a[0] = 1;
    for (size_t n = 1; n < PERIOD; ++n) {
        seq_t sum = 0;
        size_t maxk = (n <= deg) ? n : deg;
        for (size_t k = 1; k <= maxk; ++k) {
            sum += phi->coeff[k] * a[n - k];
        }
        a[n] = -sum;
    }

    for (size_t i = 0; i < PERIOD; ++i) table[i] = a[i];

    /* expose the table to the caller */
    seq_t *global = table;
    static bool ready = false;
    if (!ready) {
        ready = true;
    }
    /* The array 'table' is static, so it remains after this function */
    (void)global; /* silence unused variable warnings */
}

/* external linkage as required */
seq_t a(idx_t n)
{
    static bool inited = false;
    static seq_t period[782] = {0};

    if (!inited) {
        /* build table once */
        enum { PERIOD = 782, MAXP = 783 };
        const int divs[8] = {1, 2, 17, 23, 34, 46, 391, 782};

        struct Poly {
            size_t size;
            seq_t coeff[MAXP];
        };
        struct Poly polys[8];

        /* compute Φ_d for each divisor */
        for (int idx = 0; idx < 8; ++idx) {
            int d = divs[idx];
            struct Poly p;
            p.size = d + 1;
            for (size_t i = 0; i < p.size; ++i) p.coeff[i] = 0;
            p.coeff[0] = -1;
            p.coeff[d] = 1;

            for (int j = 0; j < idx; ++j) {
                int e = divs[j];
                if (d % e == 0) {
                    seq_t tmp[MAXP];
                    size_t qsz = poly_div(p.coeff, p.size,
                                          polys[j].coeff, polys[j].size,
                                          tmp);
                    p.size = qsz;
                    for (size_t k = 0; k < qsz; ++k) p.coeff[k] = tmp[k];
                }
            }
            polys[idx] = p;
        }

        const struct Poly *phi = &polys[7];
        size_t deg = phi->size - 1;

        seq_t a[PERIOD];
        a[0] = 1;
        for (size_t n = 1; n < PERIOD; ++n) {
            seq_t sum = 0;
            size_t maxk = (n <= deg) ? n : deg;
            for (size_t k = 1; k <= maxk; ++k) {
                sum += phi->coeff[k] * a[n - k];
            }
            a[n] = -sum;
        }
        for (size_t i = 0; i < PERIOD; ++i) period[i] = a[i];

        inited = true;
    }

    return period[(size_t)(n % 782)];
}
