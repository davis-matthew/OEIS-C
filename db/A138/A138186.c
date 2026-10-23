/*
 * A138186  A triangular sequence based on expansion of the rational
 *          polynomial of A023054 as a Sheffer sequence: p(x,t)=Exp[x*t]*(1 -
 *          t^5)/((1 - t)*(1 - t^2)^2*(1 - t^3)).
 * https://oeis.org/A138186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, uned, tabl
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A138186
 * a(n) = coefficient in row r = floor((sqrt(8*(n-1)+1)-1)/2) of the triangular array
 *          where each row r (0‑based) is given by
 *          P_r(x) = sum_{k=0}^{r} binom(r, k) * R_{r-k} * x^k
 *          and the rational series R(t) satisfies
 *          (1 - t^5) = ((1 - t)(1 - t^2)^2(1 - t^3)) * R(t)
 *          with R(t) = sum_{m>=0} R_m t^m.
 *
 * The coefficients are extracted as
 *   a(n) = fac[r]/fac[m] * R_{r-m}
 *   where m = n - r(r+1)/2 - 1 and fac[0] = 1.
 */
#include <stdlib.h>
#include <stdint.h>

/* integer square root (floor) ------------------------------------------------ */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t s = 0;
    while ((s + 1) * (s + 1) <= n) ++s;
    return s;
}

/* recurrence coefficients for (1 - t^5) = D(t) * R(t) ----------------------- */
static const int D[9] = { 1, -1, -2,  1,  2,  1, -2, -1,  1 };

/* static cache --------------------------------------------------------------- */
static idx_t cache_size = 0;
static seq_t *R = NULL;   /* coefficients of R(t) */
static seq_t *fac = NULL; /* factorials */

/* --------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safety, offset starts at 1 */

    /* compute row index r ----------------------------------------------- */
    idx_t t = 8ULL * (n - 1) + 1;
    idx_t r = (isqrt_u64(t) - 1) / 2;

    /* enlarge cache if necessary ---------------------------------------- */
    if (r >= cache_size) {
        idx_t new_size = r + 1;
        R   = (seq_t*)realloc(R,  new_size * sizeof(seq_t));
        fac = (seq_t*)realloc(fac, new_size * sizeof(seq_t));

        /* compute missing R coefficients ---------------------------------- */
        for (idx_t i = cache_size; i < new_size; ++i) {
            seq_t pos_sum = 0, neg_sum = 0;
            idx_t kmax = (i < 8) ? i : 8;
            for (idx_t k = 1; k <= kmax; ++k) {
                int dk = D[k];
                seq_t val = R[i - k];
                if (dk > 0) pos_sum += (seq_t)dk * val;
                else        neg_sum += (seq_t)(-dk) * val;
            }
            seq_t temp = neg_sum;
            temp -= pos_sum;                 /* guaranteed non‑negative */
            if (i == 0) temp += 1;           /* f_0 = 1 */
            else if (i == 5) temp -= 1;      /* f_5 = -1 */
            R[i] = temp;
        }

        /* compute factorials up to r ------------------------------------- */
        fac[0] = 1;
        for (idx_t i = 1; i < new_size; ++i)
            fac[i] = fac[i - 1] * (seq_t)i;

        cache_size = new_size;
    }

    /* column index inside the row --------------------------------------- */
    idx_t left = r * (r + 1) / 2;
    idx_t m = n - left - 1;   /* 0‑based column */

    /* answer: fac[r]/fac[m] * R[r-m] ------------------------------------ */
    seq_t div = fac[r] / fac[m];
    seq_t ans = div * R[r - m];
    return ans;
}
