/*
 * A016345  174th cyclotomic polynomial.
 * https://oeis.org/A016345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   not stated
 * Range:     i8 n <= 56, i16 n <= 56, i32 n <= 56, i64 n <= 56, i128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coefficients of the 174th cyclotomic polynomial: a(n) is the
 * coefficient of x^n in Φ₁₇₄(x).  The polynomial has degree 56,
 * so a(n)=0 for n>56. */
#include <stddef.h>
#include <stdbool.h>

/* Helper: divide monic polynomial num by monic polynomial den.
 * num is of degree deg_num, den of degree deg_den, both monic.
 * The quotient is stored in quot (at least deg_num‑deg_den+1 elements),
 * and its degree in deg_quot.  All arrays must be large enough (<=200). */
static void poly_div_monic(const seq_t *num, size_t deg_num,
                           const seq_t *den, size_t deg_den,
                           seq_t *quot, size_t *deg_quot)
{
    seq_t work[200];
    for (size_t i = 0; i <= deg_num; ++i) work[i] = num[i];

    for (size_t i = 0; i <= deg_num - deg_den; ++i) quot[i] = 0;

    for (int i = (int)deg_num; i >= (int)deg_den; --i) {
        seq_t coeff = work[i];
        if (coeff != 0) {
            int pos = i - (int)deg_den;
            quot[pos] = coeff;

            for (size_t j = 0; j <= deg_den; ++j) {
                work[pos + j] -= coeff * den[j];
            }
        }
    }

    *deg_quot = deg_num - deg_den;
    while (*deg_quot > 0 && quot[*deg_quot] == 0) (*deg_quot)--;
}

/* Compute all cyclotomic polynomials Φ_d(x) for d | 174 in ascending
 * order (1,2,3,6,29,58,87,174).  The coefficients of Φ₁₇₄ are kept
 * in the static arrays coeff_174 and deg_174.   The function is
 * executed once. */
static void init_phi_174(void)
{
    static bool done = false;
    if (done) return;

    const uint64_t divs[8] = {1, 2, 3, 6, 29, 58, 87, 174};
    static seq_t phi[8][57];   /* coefficients for each divisor */
    static size_t deg[8] = {0};

    for (size_t idx = 0; idx < 8; ++idx) {
        uint64_t d = divs[idx];
        seq_t cur[200] = {0};
        size_t cur_deg = d;

        /* numerator x^d – 1 */
        cur[0] = (seq_t)-1;
        cur[d] = (seq_t)1;

        /* divide by all smaller Φ_e where e | d */
        for (size_t eidx = 0; eidx < idx; ++eidx) {
            if (d % divs[eidx] == 0) {
                seq_t quot[200] = {0};
                size_t qdeg = 0;
                poly_div_monic(cur, cur_deg,
                               phi[eidx], deg[eidx],
                               quot, &qdeg);
                /* cur becomes the quotient for next division */
                for (size_t i = 0; i <= qdeg; ++i) cur[i] = quot[i];
                cur_deg = qdeg;
            }
        }

        /* store result for this divisor */
        for (size_t i = 0; i <= cur_deg; ++i) phi[idx][i] = cur[i];
        deg[idx] = cur_deg;
    }

    /* copy Φ₁₇₄ into global coefficient array */
    static seq_t coeff_174[57];
    static size_t deg_174 = 0;

    for (size_t i = 0; i <= deg[7]; ++i) coeff_174[i] = phi[7][i];
    deg_174 = deg[7];
    done = true;
}

/* Entry point used by the tester.  Returns a(n). */
seq_t a(idx_t n)
{
    static bool ready = false;
    static seq_t coeff[57] = {0};
    static size_t deg = 0;

    if (!ready) {
        init_phi_174();
        /* Retrieve coefficients of Φ₁₇₄ (degree 56). */
        for (size_t i = 0; i <= 56; ++i) coeff[i] = ((int) i <= 56 ? 0 : 0); /* dummy init */
        /* Actually get from the static inside init_phi_174 */
        /* We recompute because original function stores them in static locals.
           For simplicity, we call init_phi_174 again and copy into coeff. */
        /* (The init function already stores them in static arrays; we copy them.) */
        /* Use a local static accessor: */
        for (size_t i = 0; i <= 56; ++i) {
            /* No direct access; we have to recompute or use a helper.
               Instead, we store the polynomial in a global array inside init_phi_174.
               For that purpose, we will make the arrays static inside init_phi_174. */
        }
    }

    /* Simplification: Since init_phi_174 already keeps coefficients in private
       static arrays, we'll reimplement minimal duplication here to obtain the
       values.  The following recomputation mirrors the logic of init_phi_174
       but only for Φ₁₇₄, keeping it self‑contained. */

    /* Recompute Φ₁₇₄ solely here (numerical work is tiny). */
    {
        const uint64_t divs[8] = {1, 2, 3, 6, 29, 58, 87, 174};
        static seq_t sumPhi[8][57] = {0};
        static size_t sumDeg[8] = {0};

        /* Recompute all smaller Φs first */
        for (size_t idx = 0; idx < 8; ++idx) {
            uint64_t d = divs[idx];
            seq_t cur[200] = {0};
            size_t cur_deg = d;

            cur[0] = (seq_t)-1;
            cur[d] = (seq_t)1;

            for (size_t eidx = 0; eidx < idx; ++eidx) {
                if (d % divs[eidx] == 0) {
                    seq_t quot[200] = {0};
                    size_t qdeg = 0;
                    poly_div_monic(cur, cur_deg,
                                   sumPhi[eidx], sumDeg[eidx],
                                   quot, &qdeg);
                    for (size_t i = 0; i <= qdeg; ++i) cur[i] = quot[i];
                    cur_deg = qdeg;
                }
            }

            for (size_t i = 0; i <= cur_deg; ++i) sumPhi[idx][i] = cur[i];
            sumDeg[idx] = cur_deg;
        }

        /* Now copy Φ₁₇₄ into coeff */
        for (size_t i = 0; i <= sumDeg[7]; ++i) coeff[i] = sumPhi[7][i];
        deg = sumDeg[7];
        ready = true;
    }

    /* Return coefficient or zero if out of degree */
    if (n > deg) return (seq_t)0;
    return coeff[(size_t)n];
}
